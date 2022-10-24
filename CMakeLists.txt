# PSn00bSDK example CMake script
# (C) 2021 spicyjpeg - MPL licensed

cmake_minimum_required(VERSION 3.20)

project(
	PSn00bSDK-engine
	LANGUAGES    C CXX ASM
	VERSION      1.0.0
	DESCRIPTION  "PSn00bSDK engine"
	HOMEPAGE_URL "http://lameguy64.net/?page=psn00bsdk"
)

psn00bsdk_add_executable(engine STATIC src/main.c src/graphics.c)

psn00bsdk_add_cd_image(
	iso      # Target name
	engine # Output file name (= engine.bin + engine.cue)
	iso.xml  # Path to config file
	DEPENDS engine
)
