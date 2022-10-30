cmake_minimum_required(VERSION 3.20)

project(
	engine
	LANGUAGES    C CXX ASM
	VERSION      1.0.0
	DESCRIPTION  "my engine"
	HOMEPAGE_URL ""
)

file(GLOB TEXTURES textures/*.tim)
file(GLOB _SOURCES src/*.c)

psn00bsdk_add_executable(
	engine STATIC
	${_SOURCES}
)

psn00bsdk_target_incbin(engine PRIVATE tim_texture64 textures/TEXTURE64.TIM)
psn00bsdk_target_incbin(engine PRIVATE tim_sonylogo64 textures/SONYLOGO64.TIM)

psn00bsdk_add_cd_image(
	iso      # Target name
	template # Output file name (= engine.bin + engine.cue)
	iso.xml  # Path to config file
	DEPENDS engine
)