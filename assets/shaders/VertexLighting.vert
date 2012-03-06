attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 DiffuseMaterial;
attribute vec2 TextureCoord;

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat3 NormalMatrix;
uniform vec3 AmbientMaterial;
uniform vec3 SpecularMaterial;
uniform float Shininess;

varying vec4 DestinationColor;
varying vec2 TextureCoordOut;

void main(void)
{
   vec3 normal = normalize(Normal);
   vec3 N = normalize(NormalMatrix * normal);
   vec3 L = normalize(gl_LightSource[0].position.xyz);
   //vec3 L = normalize(vec3(1, 1, 1));
   vec3 E = vec3(0, 0, 1);
   vec3 H = normalize(L + E);
   
   float df = max(0.0, dot(N, L));
   float sf = max(0.0, dot(N, H));
   sf = pow(sf, Shininess);
   
   vec3 difmat = vec3(1,1,1);

   vec3 color = AmbientMaterial + df * difmat + sf * SpecularMaterial;
   
   DestinationColor = vec4(color, 1);
   gl_Position = Projection * Modelview * Position;
   TextureCoordOut = TextureCoord;
}
