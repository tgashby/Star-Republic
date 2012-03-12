varying vec2 TextureCoordOut;

uniform sampler2D Sampler0;
uniform sampler2D Sampler1;
uniform sampler2D Sampler2;
uniform sampler2D Sampler3;
uniform sampler2D Sampler4;

void main(void)
{
   vec4 tex0 = texture2D(Sampler0, TextureCoordOut);
   vec4 tex1 = texture2D(Sampler1, TextureCoordOut);
   vec4 tex2 = texture2D(Sampler2, TextureCoordOut);
   vec4 tex3 = texture2D(Sampler3, TextureCoordOut);
   vec4 tex4 = texture2D(Sampler4, TextureCoordOut);
   gl_FragColor = tex0 + tex1 + tex2 + tex3 + tex4;
}