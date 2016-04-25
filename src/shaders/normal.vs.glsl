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
 
const int LIGHT_COUNT = 1;          // Number of lights
varying vec3 N;			    		// Normal vector
varying vec3 V;			    		// vertex vector	
varying vec3 lightvec[LIGHT_COUNT]; // light vectors

void main(void)
{
	gl_TexCoord[0]  = gl_MultiTexCoord0;
	N          	    = normalize(gl_NormalMatrix * gl_Normal);
	V		        = vec3(gl_ModelViewMatrix * gl_Vertex);
	
	for(int i = 0; i < LIGHT_COUNT; i++)
		lightvec[i] = normalize(gl_LightSource[i].position.xyz - V);
	
	gl_Position     = gl_ModelViewProjectionMatrix * gl_Vertex;
}