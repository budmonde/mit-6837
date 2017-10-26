# This is the script we will test your submission with.

SIZE="800 800"
BIN=build/a4
OUT=myout

${BIN} -size ${SIZE} -input data/scene01_plane.txt  -output ${OUT}/a01.png -normals ${OUT}/a01n.png -depth 8 18 ${OUT}/a01d.png
${BIN} -size ${SIZE} -input data/scene02_cube.txt   -output ${OUT}/a02.png -normals ${OUT}/a02n.png -depth 8 18 ${OUT}/a02d.png
${BIN} -size ${SIZE} -input data/scene03_sphere.txt -output ${OUT}/a03.png -normals ${OUT}/a03n.png -depth 8 18 ${OUT}/a03d.png
${BIN} -size ${SIZE} -input data/scene04_axes.txt   -output ${OUT}/a04.png -normals ${OUT}/a04n.png -depth 8 18 ${OUT}/a04d.png
${BIN} -size ${SIZE} -input data/scene05_bunny_200.txt -output ${OUT}/a05.png -normals ${OUT}/a05n.png -depth 0.8 1.0 ${OUT}/a05d.png
${BIN} -size ${SIZE} -input data/scene06_bunny_1k.txt -bounces 4 -output ${OUT}/a06.png -normals ${OUT}/a06n.png -depth 8 18 ${OUT}/a06d.png
${BIN} -size ${SIZE} -input data/scene07_arch.txt -bounces 4 -shadows -output ${OUT}/a07.png -normals ${OUT}/a07n.png -depth 8 18 ${OUT}/a07d.png
