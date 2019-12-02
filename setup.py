from distutils.core import setup, Extension

keyword_list = [
    'finance',
    'stocks',
    'modeling',
    'stock market'
]

classifers_list = [
    'Development Status :: 2 - Pre-Alpha',
    'Operating System :: MacOS',
    'Operating System :: Microsoft :: Windows',
    'Operating System :: Unix'
]

module1 = Extension('_royal',
                    define_macros = [('USE_PRINTER', '1')],
                    include_dirs = ['include'],
                    sources = ['src/demo.c'])

setup (name = 'royal',
       version = '0.0.1',
       description = 'This is a demo package',
       author = 'Joshua Weinstein',
       author_email = 'jweinst1@berkeley.edu',
       url = 'https://github.com/jweinst1/Royal',
       long_description = open('README.md').read(),
       keyowrds = keyword_list,
       classifiers = classifers_list,
       ext_modules = [module1])