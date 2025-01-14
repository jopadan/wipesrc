#!/usr/bin/env python3
import ninja_syntax
import os
import shlex
import subprocess

def get_pkg_config(*args):
    out = subprocess.check_output(['pkg-config'] + list(args))
    return shlex.split(out.strip().decode())

def main():
    os.chdir(os.path.dirname(__file__))
    os.makedirs('build', exist_ok=True)

    cxxflags = [
        '-march=native',
        '-mfpmath=sse+387',
        '-mveclibabi=svml',
        '-g',
        '-O3',
        '-fno-omit-frame-pointer',
        '-std=gnu++23',
        '-Werror',
        '-Wno-deprecated-declarations',
        '-Wno-empty-body',
        '-Wno-unused-but-set-variable',
        '-Wno-unused-variable',
        '-Wno-unused-result',
        '-DWIPEOUTPC',
        '-DGLFW_INCLUDE_GLCOREARB',
    ]

    linkflags = [
        '-march=native',
        '-mfpmath=sse+387',
        '-mveclibabi=svml',
        '-g',
        '-O3',
        '-lGL',
        '-lGLU',
        '-lGLEW',
        '-lglfw',
        '-fno-omit-frame-pointer',
    ]

    cxxflags += get_pkg_config('--cflags', 'glfw3')
    linkflags += get_pkg_config('--libs', 'glfw3')

    cxx = 'g++'
    # cxx = os.path.expanduser('~/Code/include-what-you-use/build/bin/include-what-you-use')
    # cxxflags += ['-Xiwyu', '--check_also=src/*.h']

    try:
        with open('build.ninja', 'w') as f:
            ninja = ninja_syntax.Writer(f)
            ninja.rule('cxx', [cxx, '$cxxflags', '-c', '$in', '-MD', '-MF', '$out.d', '-o', '$out'], depfile='$out.d')
            ninja.rule('link', [cxx, '$linkflags', '$in', '-o', '$out'])
            ninja.variable('cxxflags', cxxflags)
            ninja.variable('linkflags', linkflags)

            objects = []

            for dirpath, dirnames, filenames in os.walk('src'):
                for filename in filenames:
                    if filename.endswith('.cpp') and not filename.endswith('_old.cpp'):
                        cpp_file = os.path.join(dirpath, filename)
                        obj_file = os.path.join('build', dirpath, filename.removesuffix('.cpp') + '.o')
                        ninja.build(obj_file, 'cxx', cpp_file)
                        objects.append(obj_file)

            ninja.build('wipeout-port', 'link', objects)
            ninja.default('wipeout-port')

        with open('build/compile_commands.json', 'wb') as f:
            subprocess.check_call(['ninja', '-t', 'compdb'], stdout=f)

        subprocess.check_call(['ninja'])
    finally:
        os.unlink('build.ninja')

if __name__ == '__main__':
    main()
