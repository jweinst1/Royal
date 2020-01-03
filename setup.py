from distutils.core import setup, Extension
import os

keyword_list = [
    'graphs',
    'graph',
    'data science',
    'big data',
    'ML'
]

classifers_list = [
    'Development Status :: 2 - Pre-Alpha',
    'Operating System :: MacOS',
    'Operating System :: Microsoft :: Windows',
    'Operating System :: Unix'
]

macro_defs = []

if os.name == 'nt':
  macro_defs.append(('_CRT_SECURE_NO_WARNINGS', '1'))

macro_defs.append(('ROYAL_MEM_DEBUG', '1'))

# Version definition
macro_defs.append(('ROYAL_VERSION', '1'))
macro_defs.append(('ROYAL_MAJOR_VERSION', '0'))
macro_defs.append(('ROYAL_MINOR_VERSION', '0'))
macro_defs.append(('ROYAL_PATCH_VERSION', '3'))

module1 = Extension('royal',
                    define_macros = macro_defs,
                    include_dirs = ['include'],
                    sources = ['src/pymain.c', 'src/royal_graphs.c'])

setup (name = 'royal',
       version = '0.0.3',
       description = 'A graph data format library',
       author = 'Joshua Weinstein',
       author_email = 'jweinst1@berkeley.edu',
       url = 'https://github.com/jweinst1/Royal',
       long_description = open('README.md').read(),
       license = "MIT",
       keywords = keyword_list,
       classifiers = classifers_list,
       ext_modules = [module1])