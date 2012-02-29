varying vec2 TextureCoordOut;

uniform sampler2D Sampler;
uniform sampler2D Sampler1;
uniform sampler2D Sampler2;
uniform sampler2D Sampler3;
uniform sampler2D Sampler4;

void main(void)
{
   vec4 pass0 = texture2D(Sampler, TextureCoordOut);
   vec4 reduce0 = texture2D(Sampler1, TextureCoordOut);
   vec4 reduce1 = texture2D(Sampler2, TextureCoordOut);
   vec4 reduce2 = texture2D(Sampler3, TextureCoordOut);
   vec4 reduce3 = texture2D(Sampler4, TextureCoordOut);
   gl_FragColor = pass0 + reduce0 + reduce1 + reduce2 + reduce3;
}