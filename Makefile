#SETTINGS (Modify this)
USEIMG := 0
USEMIX := 0
USETTF := 0
DEBUG := 0


CCV = -std=c++11
WC = emcc
WSHELL = --shell-file res/mm.html
WBIND = --bind -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1
WFILE = --preload-file demo/res --use-preload-plugins
#-lmingw32 
ifeq ($(OS),Windows_NT)
	LIB = -lSDL2main -lSDL2
else
	LIB = -lSDL2
endif

ifeq ($(DEBUG), 1)
	CCV += -g
endif
ifeq ($(USEIMG), 1)
	LIB += -lSDL2_image
	WBIND += -s USE_SDL_IMAGE=2
endif
ifeq ($(USEMIX), 1)
	LIB += -lSDL2_mixer
	WBIND += -s USE_SDL_MIXER=2
endif
ifeq ($(USETTF), 1)
	LIB += -lSDL2_ttf
	WBIND += -s USE_SDL_TTF=2
endif

SRC_FILES := $(wildcard src/*.h)
DEMO := $(wildcard demo/*.cpp*.h)

DLIST := -DMIX=$(USEMIX) -DIMG=$(USEIMG) -DTTF=$(USETTF) -DDEBUG=$(DEBUG)

ifeq ($(OS),Windows_NT)
	OUT := build/app.exe
else
	OUT := build/app
endif

IN = demo/main.cpp
LIST = -Ibuild\_deps\sdl2-src\include

pc: $(DEMO) PME.h $(SRC_FILES)
	$(CXX) $(CCV) $(CXXFLAGS) $(IN) $(LIST) -DPORT=1 $(DLIST) $(LIB) -o $(OUT)

web: $(DEMO) PME.h $(SRC_FILES)
	$(WC) $(CCV) $(IN) -DPORT=0 $(DLIST) $(WBIND) $(WSHELL) $(WFILE) -o build/index.html -s SINGLE_FILE=1 -s SDL2_IMAGE_FORMATS='["png"]'

run:
	$(OUT)

#pip3 install quom
header:
	quom PME.h dist/PME.h