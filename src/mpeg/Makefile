mp:
	g++ -o mp -DTEST_MPEG2DEC mplayerWidget.cpp  mpeg2Class.cpp -I./mpeg2dec/include -I./  -lfltk2 -lXi -lpthread -lm -lXext -lsupc++ mpeg2dec/libmpeg2/.libs/libmpeg2.a mpeg2dec/libmpeg2/convert/.libs/libmpeg2convert.a mpeg2dec/libmpeg2/convert/rgb_mmx.c  mpeg2dec/libmpeg2/convert/rgb.c 
clean:
	rm -f *.o mp
