/*
 * Copyright 2013 Sebastian Rettenberger
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
const int LIGHT_COUNT = 1;
varying vec3 N;
varying vec3 V;
varying vec3 lightvec[LIGHT_COUNT];
uniform sampler2D Texture0;
 
void main(void)
{
	vec2 TexCoord = vec2(gl_TexCoord[0]);
	
	vec3 Eye    = normalize(-V);
	vec3 normal = N;
 
	vec4 EndColor = vec4(0.0, 0.0, 0.0, 0.0);
	vec4 EndSpec  = vec4(0.0, 0.0, 0.0, 0.0);
	for(int i = 0; i < LIGHT_COUNT; i++){
		vec3 Reflected = normalize(reflect(-lightvec[i], normal)); 
		vec4 IAmbient  = gl_LightSource[i].ambient  * gl_FrontMaterial.ambient;
		vec4 IDiffuse  = gl_LightSource[i].diffuse  * gl_FrontMaterial.diffuse  * max(dot(normal, lightvec[i]), 0.0);
		vec4 ISpecular = gl_LightSource[i].specular * gl_FrontMaterial.specular * pow(max(dot(Reflected, Eye), 0.0), gl_FrontMaterial.shininess);
		EndColor += (IAmbient+IDiffuse);
		EndSpec  += ISpecular;
	}
	//EndColor += gl_FrontMaterial.emission;
 
	gl_FragColor = (gl_FrontLightModelProduct.sceneColor + EndColor) * texture2D(Texture0, TexCoord) + EndSpec;
}