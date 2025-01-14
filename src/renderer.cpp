#include "renderer.h"
#include "libgpu.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#define CHECK_GL_ERROR() checkGLError(__func__)

static const char VERTEX_SHADER[] = R"glsl(
  #version 410 core

  layout(location = 0) in vec2 position;
  layout(location = 1) in vec3 color;
  layout(location = 2) in vec2 texcoord;

  out vec3 colorFrag;
  out vec2 texcoordFrag;

  void main() {
    gl_Position = vec4(position.x / 160.0 - 1, 1 - position.y / 120.0, 0.0, 1.0);
    colorFrag = color;
    texcoordFrag = texcoord;
  }
)glsl";

static const char FRAGMENT_SHADER[] = R"glsl(
  #version 410 core

  in vec3 colorFrag;
  in vec2 texcoordFrag;

  out vec4 outColor;

  uniform int textured;
  uniform sampler2D sampler;

  void main() {
    if (textured == 0) {
      outColor = vec4(1.0); //vec4(colorFrag, 1.0);
    } else {
      outColor = texture(sampler, texcoordFrag); // * vec4(colorFrag, 1.0);
    }
  }
)glsl";

struct RenderData {
  std::vector<GLshort> vertices;
  std::vector<GLubyte> colors;
  std::vector<GLfloat> texcoords;
};

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

extern char VRam[];
extern bool vramDirty;

static const size_t NUM_PAGES = 32;
static const GLint POSITION_ATTRIB = 0;
static const GLint COLOR_ATTRIB = 1;
static const GLint TEXCOORD_ATTRIB = 2;
static GLuint g_program = 0;
static GLuint g_vertexBuffer = 0;
static GLuint g_colorBuffer = 0;
static GLuint g_texcoordBuffer = 0;
static GLuint g_vao = 0;
static GLuint g_vramTex = 0;
static RenderData g_renderData;
static RenderData g_renderDataTextured;
static Color g_palette[256];

static void checkGLError(const char* func) {
  GLenum err;
  while((err = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL error in %s: %d\n", func, err);
  }
}

static GLuint createShader(GLenum type, const char* source) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if(status == GL_TRUE) {
    return shader;
  }

  char buffer[512];
  glGetShaderInfoLog(shader, sizeof(buffer), nullptr, buffer);
  fprintf(stderr, "Failed to compile shader %d: %s\n", type, buffer);
  exit(1);
}

static GLuint createProgram(const char* vertSource, const char* fragSource) {
  GLuint vertShader = createShader(GL_VERTEX_SHADER, vertSource);
  GLuint fragShader = createShader(GL_FRAGMENT_SHADER, fragSource);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);

  glDeleteShader(vertShader);
  glDeleteShader(fragShader);

  CHECK_GL_ERROR();
  return program;
}

void rendererInit() {
  g_program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
  glUseProgram(g_program);

  glGenVertexArrays(1, &g_vao);
  glBindVertexArray(g_vao);

  glGenBuffers(1, &g_vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);
  glVertexAttribPointer(POSITION_ATTRIB, 2, GL_SHORT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(POSITION_ATTRIB);
  checkGLError("g_vertexBuffer");

  glGenBuffers(1, &g_colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, g_colorBuffer);
  glVertexAttribPointer(COLOR_ATTRIB, 3, GL_UNSIGNED_BYTE, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(COLOR_ATTRIB);
  checkGLError("g_colorBuffer");

  glGenBuffers(1, &g_texcoordBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, g_texcoordBuffer);
  glVertexAttribPointer(TEXCOORD_ATTRIB, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(TEXCOORD_ATTRIB);
  checkGLError("g_texcoordBuffer");

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glCullFace(GL_FRONT_AND_BACK); // Culling off by default
  glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // The default
  // glActiveTexture(GL_TEXTURE0);

  // Also the default
  // GLint samplerLoc = glGetUniformLocation(g_program, "sampler");
  // glUniform1i(samplerLoc, 0);

  for (size_t i = 0; i < sizeof(g_palette) / sizeof(g_palette[0]); i++) {
    g_palette[i].r = i;
    g_palette[i].g = i;
    g_palette[i].b = i;
  }

  CHECK_GL_ERROR();
}

void rendererTerm() {
}

static void renderWithMode(RenderData& renderData, bool textured) {
  if (renderData.vertices.empty()) {
    return;
  }

  GLint texturedLoc = glGetUniformLocation(g_program, "textured");
  glUniform1i(texturedLoc, textured);

  glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER, renderData.vertices.size() * sizeof(GLshort), renderData.vertices.data(), GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, g_colorBuffer);
  glBufferData(GL_ARRAY_BUFFER, renderData.colors.size() * sizeof(GLubyte), renderData.colors.data(), GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, g_texcoordBuffer);
  glBufferData(GL_ARRAY_BUFFER, renderData.texcoords.size() * sizeof(GLfloat), renderData.texcoords.data(), GL_STREAM_DRAW);

  glDrawArrays(GL_TRIANGLES, 0, renderData.vertices.size() / 2);

  renderData.vertices.clear();
  renderData.colors.clear();
  renderData.texcoords.clear();
}

static void loadVRamToTexture() {
  if(!vramDirty) {
    return;
  }

  vramDirty = false;

  if(g_vramTex == 0) {
    glGenTextures(1, &g_vramTex);
  }

  GLint level = 0;
  GLint internalFormat = GL_RGB;
  GLsizei width = 256;
  GLsizei height = 256 * 32;
  GLint border = 0;
  GLenum format = GL_RGB;
  GLenum type = GL_UNSIGNED_BYTE;
  const void* pixels = VRam;

  glBindTexture(GL_TEXTURE_2D, g_vramTex);
  glTexImage2D(GL_TEXTURE_2D, level, internalFormat, width, height, border, format, type, pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  CHECK_GL_ERROR();
}

void rendererRender() {
  loadVRamToTexture();
  glClear(GL_COLOR_BUFFER_BIT);
  renderWithMode(g_renderData, false);
  renderWithMode(g_renderDataTextured, true);
  CHECK_GL_ERROR();
}

#define BLACK2_TRANSL 3
#define WHITE2_TRANSL 9
#define RED_LIGHT_FADE 16

static Color getColor(uint8_t r, uint8_t g, uint8_t b) {
  if (g) {
    if (g == 25) {
      return { 40, 40, 255 }; // Speed up
    } else {
      // int32_t r = DepthFadeTble[(g << 8) + WhitePal];
      // int32_t r = 128; // FIXME
      // return g_palette[r];
      return { 255, 255, 255 };
    }
  } else if (b) {
    switch (b) {
    case BLACK2_TRANSL:
      return { 180, 180, 180 };
    case RED_LIGHT_FADE:
      return { 255, 10, 0 };
    case WHITE2_TRANSL:
    default:
      return { 100, 100, 100 };
    }
  } else {
    return g_palette[r];
  }
}

static GLfloat calcTexCoordU(uint16_t tpage, uint8_t u) {
  return (GLfloat)u / (GLfloat)0xFF;
}

static GLfloat calcTexCoordV(uint16_t tpage, uint8_t v) {
  return (GLfloat)(tpage & 0x1F) / (GLfloat)NUM_PAGES + (GLfloat)v / (GLfloat)(0xFF * NUM_PAGES);
}

struct VertPos {
  int16_t x, y;
};

struct VertColor {
  uint8_t r, g, b;
};

struct VertTex {
  uint8_t u, v;
};

template<size_t N>
struct PrimIndices;

template<>
struct PrimIndices<3> {
  static constexpr size_t INDICES[] = { 0, 1, 2 };
};

template<>
struct PrimIndices<4> {
  static constexpr size_t INDICES[] = { 0, 1, 2, /**/ 0, 2, 3 };
};

template<size_t N>
static void addPrim(
  std::array<VertPos, N> pos,
  std::array<VertColor, N> color)
{
  for(size_t i : PrimIndices<N>::INDICES) {
    g_renderData.vertices.push_back(pos[i].x);
    g_renderData.vertices.push_back(pos[i].y);
    Color transColor = getColor(color[i].r, color[i].g, color[i].b);
    g_renderData.colors.push_back(transColor.r);
    g_renderData.colors.push_back(transColor.g);
    g_renderData.colors.push_back(transColor.b);
    g_renderData.texcoords.push_back(0.0f);
    g_renderData.texcoords.push_back(0.0f);
  }
}

template<size_t N>
static void addPrim(
  std::array<VertPos, N> pos,
  std::array<VertColor, N> color,
  std::array<VertTex, N> tex,
  uint16_t texpage)
{
  for(size_t i : PrimIndices<N>::INDICES) {
    g_renderDataTextured.vertices.push_back(pos[i].x);
    g_renderDataTextured.vertices.push_back(pos[i].y);
    Color transColor = getColor(color[i].r, color[i].g, color[i].b);
    g_renderDataTextured.colors.push_back(transColor.r);
    g_renderDataTextured.colors.push_back(transColor.g);
    g_renderDataTextured.colors.push_back(transColor.b);
    g_renderDataTextured.texcoords.push_back(calcTexCoordU(texpage, tex[i].u));
    g_renderDataTextured.texcoords.push_back(calcTexCoordV(texpage, tex[i].v));
  }
}

static void addSprite(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t u, uint8_t v, uint16_t tpage) {
  x -= 160;
  // y -= 120;
  addPrim<4>(
    {
      VertPos { x, y },
      VertPos { (int16_t)(x + w), y },
      VertPos { (int16_t)(x + w), (int16_t)(y + h) },
      VertPos { x, (int16_t)(y + h) },
    },
    {
      VertColor { 255, 255, 255 },
      VertColor { 255, 255, 255 },
      VertColor { 255, 255, 255 },
      VertColor { 255, 255, 255 },
    },
    {
      VertTex { u, v },
      VertTex { (uint8_t)(u + w), v },
      VertTex { (uint8_t)(u + w), (uint8_t)(v + h) },
      VertTex { u, (uint8_t)(v + h) },
    },
    tpage
  );
}

/*
 * Begin implemented game functions
 */

void CopyPal(char* palette, int16_t start, int16_t end) {
  for(int16_t i = start; i <= end; i++) {
    g_palette[i].r = *(uint8_t*)palette++;
    g_palette[i].g = *(uint8_t*)palette++;
    g_palette[i].b = *(uint8_t*)palette++;
  }
}

void JJS_Sprite(SPRT* sp) {
  addSprite(sp->x0, sp->y0, sp->w, sp->h, sp->u0, sp->v0, sp->clut);
}

void JJS_Sprite8(SPRT_8* sp) {
  addSprite(sp->x0, sp->y0, 8, 8, sp->u0, sp->v0, sp->clut);
}

void JJSDrawPolyF3(POLY_F3* p) {
  addPrim<3>(
    {
      VertPos { p->x0, p->y0 },
      VertPos { p->x1, p->y1 },
      VertPos { p->x2, p->y2 },
    },
    {
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
    });
}

void JJSDrawPolyF4(POLY_F4* p) {
  addPrim<4>(
    {
      VertPos { p->x0, p->y0 },
      VertPos { p->x1, p->y1 },
      VertPos { p->x3, p->y3 },
      VertPos { p->x2, p->y2 },
    },
    {
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
    });
}

void JJSDrawPolyG3(POLY_G3* p) {
  addPrim<3>(
    {
      VertPos { p->x0, p->y0 },
      VertPos { p->x1, p->y1 },
      VertPos { p->x2, p->y2 },
    },
    {
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r1, p->g1, p->b1 },
      VertColor { p->r2, p->g2, p->b2 },
    });
}

void JJSDrawPolyG4(POLY_G4* p) {
  addPrim<4>(
    {
      VertPos { p->x0, p->y0 },
      VertPos { p->x1, p->y1 },
      VertPos { p->x3, p->y3 },
      VertPos { p->x2, p->y2 },
    },
    {
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r1, p->g1, p->b1 },
      VertColor { p->r3, p->g3, p->b3 },
      VertColor { p->r2, p->g2, p->b2 },
    });
}

void JJSDrawPolyFT3(POLY_FT3* p) {
  addPrim<3>(
    {
      VertPos { p->x0, p->y0 },
      VertPos { p->x1, p->y1 },
      VertPos { p->x2, p->y2 },
    },
    {
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
    },
    {
      VertTex { p->u0, p->v0 },
      VertTex { p->u1, p->v1 },
      VertTex { p->u2, p->v2 },
    },
    p->tpage);
}

void JJSDrawPolyFT4(POLY_FT4* p) {
  addPrim<4>(
    {
      VertPos { p->x0, p->y0 },
      VertPos { p->x1, p->y1 },
      VertPos { p->x3, p->y3 },
      VertPos { p->x2, p->y2 },
    },
    {
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r0, p->g0, p->b0 },
    },
    {
      VertTex { p->u0, p->v0 },
      VertTex { p->u1, p->v1 },
      VertTex { p->u3, p->v3 },
      VertTex { p->u2, p->v2 },
    },
    p->tpage);
}

void JJSDrawPolyGT3(POLY_GT3* p) {
  addPrim<3>(
    {
      VertPos { p->x0, p->y0 },
      VertPos { p->x1, p->y1 },
      VertPos { p->x2, p->y2 },
    },
    {
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r1, p->g1, p->b1 },
      VertColor { p->r2, p->g2, p->b2 },
    },
    {
      VertTex { p->u0, p->v0 },
      VertTex { p->u1, p->v1 },
      VertTex { p->u2, p->v2 },
    },
    p->tpage);
}

void JJSDrawPolyGT4(POLY_GT4* p) {
  addPrim<4>(
    {
      VertPos { p->x0, p->y0 },
      VertPos { p->x1, p->y1 },
      VertPos { p->x3, p->y3 },
      VertPos { p->x2, p->y2 },
    },
    {
      VertColor { p->r0, p->g0, p->b0 },
      VertColor { p->r1, p->g1, p->b1 },
      VertColor { p->r3, p->g3, p->b3 },
      VertColor { p->r2, p->g2, p->b2 },

    },
    {
      VertTex { p->u0, p->v0 },
      VertTex { p->u1, p->v1 },
      VertTex { p->u3, p->v3 },
      VertTex { p->u2, p->v2 },
    },
    p->tpage);
}

/*
 * End implemented game functions
 */
