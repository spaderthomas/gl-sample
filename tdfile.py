import tdbuild.tdbuild as tdbuild

build_options = {
    'source_dir': 'src',
    'include_dirs': [
        'include'
    ],
    'lib_dir': 'lib',
    'build_dir': 'build',
    'source_files': [
        'glad.c',
        'main.cpp'
    ],
    'debug': True,
    'cpp': True,
    'cpp_standard': '17',
    'Linux': {
        'compiler': 'g++',
        'user_libs': [
            'glfw'
        ],
        'system_libs':[
            'GL',
            'X11',
            'c',
            'dl'
        ],
        'extras': [],
        'out': 'gl-sample'
    }
}

class Builder(tdbuild.base_builder):
    def __init__(self):
        super().__init__()

    def build(self):
        super().build()
        
    def run(self):
        super().run()
        
    def setup(self):
        super().setup()
        
    def prebuild(self):
        pass
    
