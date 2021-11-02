#version 330 core

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D colorTex;

void main()
{
	outColor = texture(colorTex, texCoord);
	if ((0.25 <= texCoord.s) && (texCoord.s <= 0.75) && (0.24 <= texCoord.t) && (texCoord.t  <= 0.26)) outColor = vec4(1.0, 0.0, 1.0, 0.0);
	else if ((0.24 <= texCoord.s) && (texCoord.s <= 0.26) && (0.25 <= texCoord.t) && (texCoord.t  <= 0.75)) outColor = vec4(1.0, 0.0, 1.0, 0.0);
	else if ((0.74 <= texCoord.s) && (texCoord.s <= 0.76) && (0.25 <= texCoord.t) && (texCoord.t  <= 0.75)) outColor = vec4(1.0, 0.0, 1.0, 0.0);
	else if ((0.25 <= texCoord.s) && (texCoord.s <= 0.75) && (0.74 <= texCoord.t) && (texCoord.t  <= 0.76)) outColor = vec4(1.0, 0.0, 1.0, 0.0);

	//else if (texCoord.t == (0.5*texCoord.s-0.0625) / 0.25) outColor = vec4(1.0, 0.0, 1.0, 0.0);
	//else if (texCoord.t == (0.5*texCoord.s-0.4375) / -0.25) outColor = vec4(1.0, 0.0, 1.0, 0.0);

	else if (outColor.rgb == vec3(0)) discard;
	//if ((0.25 <= texCoord.s) && (texCoord.s <= 0.75)) outColor = vec4(1.0, 0.0, 1.0, 0.0);
	else outColor = vec4(1);
	//outColor = vec4(texCoord, vec2(1));
}
