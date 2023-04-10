####################################################
#                                                  #
#  But God, being rich in mercy, because of the    #
#  great love with which he loved us, even when    #
#  we were dead in our trespasses, made us alive   #
#  together with Christ - by grace you             #
#  have been saved                                 #
#                                                  #
#      Ephesians 2:4-5                             #
#                                                  #
####################################################

C				:= gcc
CXX				:= g++
LINT			:= cpplint --root=..

INCLUDES		:= inc

OPTFLAGS		:= -Ofast -ffast-math \
				   -ffunction-sections \
				   -fmerge-all-constants -Wl,--gc-sections \
				   -flto -fomit-frame-pointer \

DEBUGOPTFLAGS	:= -g

LIBS			:= -lrt -pthread
LIBDIRS			:= .

SOURCES			:= src



export INCLUDE	:= $(foreach dir, $(INCLUDES), -I$(dir))

C_SRCS			:= $(foreach f, $(SOURCES), $(notdir $(wildcard $(f)/*.c)))
CC_SRCS			:= $(foreach f, $(SOURCES), $(notdir $(wildcard $(f)/*.cc)))
CXX_SRCS		:= $(foreach f, $(SOURCES), $(notdir $(wildcard $(f)/*.cpp)))

OBJS			:= $(foreach f, $(C_SRCS), $(notdir $(f:.c=.o)))
OBJS			:= $(OBJS) $(foreach f, $(CC_SRCS), $(notdir $(f:.cc=.o)))
OBJS			:= $(OBJS) $(foreach f, $(CXX_SRCS), $(notdir $(f:.cpp=.o)))

LDFLAGS			:= -L$(LIBDIRS) $(LIBS)

FLAGS			:= $(INCLUDE) $(DEBUGOPTFLAGS) -Wno-implicit-function-declaration
CFLAGS			:= -std=c17 $(FLAGS)
CXXFLAGS		:= -std=c++17 $(FLAGS)

VPATH			:= $(foreach dir, $(SOURCES), $(dir)) $(foreach dir, $(INCLUDES), $(dir))



.PHONY: all unix lint clean run



all: unix

unix: shm_sup_server shm_sup_client



shm_sup_server: shm_sup_server.o
	$(CXX) $< -o $(@:.o=) $(LDFLAGS)

shm_sup_client: shm_sup_client.o
	$(CXX) $< -o $(@:.o=) $(LDFLAGS)



lint:
	cpplint --root=.. src/* inc/*

clean:
	@echo clean ...
	
	@rm -f *.o
	
	@rm -f shm_sup_client
	@rm -f shm_sup_server
