#pragma once

#include "Texture.h"
#include "Resource.h"
#include "Core.h"

class EngineAPI TextureSampler
{
public:
	TextureSampler();

	Resource<Texture> texture;

	enum Color {
		None = 0,       // 0000
		R = 1 << 0,		// 0001
		G = 1 << 1,		// 0010
		B = 1 << 2,		// 0100
		A = 1 << 3,		// 1000
		RGB = R | G | B,
		RGBA = R | G | B | A
	};

	int channelMaskR = Color::R;
	int channelMaskG = Color::G;
	int channelMaskB = Color::B;
	int channelMaskA = Color::A;

	float xOffset = 0;
	float yOffset = 0;

	float xScale = 1;
	float yScale = 1;

	template <class Archive>
	void serialize(Archive& archive) {
		archive(texture, colorMask, xOffset, yOffset, xScale, yScale);
	}
};