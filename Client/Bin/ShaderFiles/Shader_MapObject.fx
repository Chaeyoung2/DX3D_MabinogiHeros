matrix			g_matWorld, g_matView, g_matProj;

vector			g_vCameraPos;

vector			g_vLightDir;
vector			g_vLightDiffuse = vector(0.3f, 0.3f, 0.3f, 1.f);
vector			g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightSpecular = vector(0.5f, 0.5f, 0.5f, 1.f);

vector			g_vMtrlDiffuse = vector(0.9f, 0.9f, 1.f, 1.f);
vector			g_vMtrlAmbient = vector(0.9f, 0.9f, 1.f, 1.f);
vector			g_vMtrlSpecular = vector(0.5f, 0.5f, 0.5f, 1.f);


float			g_fPower = 5.f;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR0;
	float2		vTexUV : TEXCOORD0;
	vector		vSpecular : TEXCOORD1;

};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0.f;

	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// 
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

	vector		vWorldNormal = mul(vector(In.vNormal, 0.f), g_matWorld);

	Out.vShade = max(dot(normalize(g_vLightDir) * -1.f
		, normalize(vWorldNormal)), 0.0f);

	vector		vReflect = reflect(g_vLightDir, normalize(vWorldNormal));

	vector		vWorldPos = mul(vector(In.vPosition, 1.f), g_matWorld);
	vector		vLook = vWorldPos - g_vCameraPos;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), g_fPower);

	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector		vPosition : POSITION;
	vector		vShade : COLOR;
	float2		vTexUV : TEXCOORD0;
	vector		vSpecular : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;


	vector		vTexCol = tex2D(BaseSampler, In.vTexUV);
	//vector		vTexCol = (vector)0.f;

	Out.vColor = vTexCol * (g_vLightDiffuse * g_vMtrlDiffuse) * (In.vShade
		+ (g_vLightAmbient * g_vMtrlAmbient)) + (g_vLightSpecular * g_vMtrlSpecular) * In.vSpecular;

	Out.vColor.a = vTexCol.a;

	return Out;
}

PS_OUT PS_MAIN_NOT_LIGHTING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0.0f;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	return Out;
}

technique DefaultDevice
{
	pass DefaultRendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass AlphaTestRendering
	{
//		lighting = true;
		AlphaTestEnable = true;
		AlphaFunc = greater;
		AlphaRef = 0x4f;

		Cullmode = none;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}