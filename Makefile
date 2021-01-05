all:
	g++ Source/*.cpp -o hw3 -std=c++11 -lXi -lGLEW -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp

horseandmug:
	./hw3 Samples/horse_and_mug.xml
full:
	make all
	make horseandmug
justhorse:
	./hw3 Samples/horse.xml
horse_and_mug_reversed:
	./hw3 Samples/horse_and_mug_reversed.xml
horse_and_mug_reversed_frontface_culled:
	./hw3 Samples/horse_and_mug_reversed_frontface_culled.xml
horse_and_mug_frontface_culled:
	./hw3 Samples/horse_and_mug_frontface_culled.xml
horse_and_mug_backface_culled:
	./hw3 Samples/horse_and_mug_backface_culled.xml
