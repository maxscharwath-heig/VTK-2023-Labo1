
ffmpeg -framerate 30 -pattern_type glob -i '*.tga' -c:v libx264 -pix_fmt yuv420p out.mp4