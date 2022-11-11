ifeq ($(OS),Windows_NT)
  include MakefileWindows
else
  include MakefileLinux
endif