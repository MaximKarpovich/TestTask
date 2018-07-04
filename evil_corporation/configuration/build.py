from utils.utils import Guide
from utils.utils import Config
from utils.utils import Executor
from utils.utils import Arguments
import os

os.system('conan remote add bincraftes https://api.bintray.com/conan/bincrafters/public-conan')
os.system('conan remote add conan_community https://api.bintray.com/conan/conan-community/conan')

guide = Guide('../..','build/')
guide.make_build()
guide.cd()

cfg = Config()
ide = cfg.get_ide()

Executor.run(['conan', 'install', '-g', 'cmake_multi', guide.root_dir , '--profile', '../conan_profile', '--build=outdated', '-s', 'build_type=Debug'])
Executor.run(['conan', 'install', '-g', 'cmake_multi', guide.root_dir , '--profile', '../conan_profile', '--build=outdated', '-s', 'build_type=Release'])
Executor.run(['cmake', guide.root_dir, '-G', ide])

guide.cd()
Executor.run(['cmake', '--build', '.', '--config', 'Release', '--', '/m:8', '/p:BuildInParallel=true'])
        