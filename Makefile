
include_path = $(shell pwd)/include

source_path  = $(shell pwd)/source

objs_path    = $(shell pwd)/obj

exec_path    = $(shell pwd)/build

src := $(shell find source -name '*.c')

inc := $(shell find include -name '*.h')

objs := $(patsubst source/%.c,obj/%.o,$(src))

compile_option := -g -Wall -I $(include_path)

# 我们尝试添加一个生成object的依赖选项 $< 右边的 $@左边的

obj/%.o : source/%.c
	mkdir -p $(dir $@)
	@# -c 选项会编译成object文件 不进行链接
	@echo $< 
	@echo $@
	cc -c $< $(compile_option) -o $@

clean :
	rm -rf obj
	rm -rf build

compile : $(objs)

debug :
	@echo $(include_path)
	@echo $(src)
	@echo $(inc)
	@echo $(objs)

client : compile
	mkdir -p  build
	cc $(compile_option) $(objs_path)/TCPechoclientDualStack.o $(objs_path)/TCPUtility.o $(objs_path)/errorPrint.o  $(objs_path)/printDNSAddr.o -o $(exec_path)/client.o 

dns: compile
	mkdir -p  build
	cc $(compile_option) $(objs_path)/DNSResolution.o $(objs_path)/errorPrint.o $(objs_path)/printDNSAddr.o -o $(exec_path)/dns.o 

server : compile
	mkdir -p  build
	cc $(compile_option) $(objs_path)/TCPechoserverv4.o $(objs_path)/errorPrint.o $(objs_path)/TCPUtility.o $(objs_path)/printDNSAddr.o -o $(exec_path)/server.o 

.PHONY : clean debug