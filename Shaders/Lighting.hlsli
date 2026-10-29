#pragma once

float3 getLightVector(Light light, float3 position, out float attenuation)
{
	float3 L;
	attenuation = 1.0f;
	
	switch (light.type)
	{
		case LIGHT_DIRECTIONAL:
			L = normalize(-light.direction);
			break;
		case LIGHT_POINT:
			L = normalize(light.position - position);
			break;
		case LIGHT_SPOT:
			L = normalize(light.position - position_;
			attenuation = smoothstep(light.outerCutoff, light.innerCutoff, dot(-L, light.direction));
			break;
	}
	
}
