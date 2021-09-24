
// �������� : Ŭ���̾�Ʈ�κ��� ���� ���� �� �ִ�, ���
// �������� �������� ��Ƽ� ������̺�(������Ʈ���̺�)
matrix			g_matWorld, g_matView, g_matProj;
float				g_fCountX;
float				g_fCountY;
float				g_fSize;
vector			g_vecFrame;
vector			g_vecMaxFrame;

texture			g_BaseTexture;

sampler BaseSampler = sampler_state
{
	texture = g_BaseTexture;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD;
};

struct VS_OUT
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	//float2		vTexFrame : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	// ���ÿ��� ��ǥ ����.

	// 0 ~ 191 / 192 ~ 383 / 384 ~ 575 / 576 ~ 767 / 768 ~ 959 
	//for (int x = 0; x < g_fCountX; ++x)
	//{
	//	if (In.vPosition.x < g_fSize * ((float)x + 1.f) && In.vPosition.x >= (float)x)
	//	{
	//		for (int y = 0; y < g_fCountY; ++y)
	//		{
	//			if (In.vPosition.y < g_fSize * ((float)y + 1.f) && In.vPosition.y >= (float)y)
	//			{
	//				Out.vTexFrame = (float)x + (float)y * g_fCountX;
	//			}
	//		}
	//	}
	//}

	//for (int x = 0; x < g_fCountX; ++x)
	//{
	//	if (In.vPosition.x < g_fSize * ((float)x + 1.f) && In.vPosition.x >= (float)x)
	//	{
	//		Out.vTexFrame.x = (float)x;
	//	}
	//}
	//for (int y = 0; y < g_fCountY; ++y)
	//{
	//	if (In.vPosition.y < g_fSize * ((float)y + 1.f) && In.vPosition.y >= (float)y)
	//	{
	//		Out.vTexFrame.y = (float)y;
	//	}
	//}


	matrix			matWV;
	matrix			matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	//float2		vTexFrame : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : COLOR;
};

// �ȼ� ���̴��� �����ϱ����� �Լ�.
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0.f;

	float2 vTexUV = In.vTexUV;

	vTexUV.x = vTexUV.x / g_fCountX + g_vecFrame.x;
	vTexUV.y = vTexUV.y / g_fCountY + g_vecFrame.y;


	//vTexUV.x +=  g_vecFrame.x;
	//vTexUV.y += g_vecFrame.y;



	//
	//vTexUV.u /= g_fCountX;
	//vTexUV.v /= g_fCountY;

	//vTexUV.u += In.vTexFrame.x * (1 / g_fSize);
	//vTexUV.v += In.vTexFrame.x * (1 / g_fSize);

	Out.vColor = tex2D(BaseSampler, vTexUV);

	/*Out.vColor.a = 1.f - In.vTexUV.y;*/


	//// �ȼ��� ���� �����Ѵ�.
	//if(In.vTexUV.y > 0.5f)
	//	Out.vColor = vector(1.f, 1.f, 1.f, 1.f);
	//else
	//	Out.vColor = vector(1.f, 0.0f, 0.0f, 1.f);\]
	return Out;
}

technique Default_Device // ��ġ�� ���̴� ���� �������ο����� ����.
{
	pass Default_Rendering
	{
		cullmode = none;
		lighting = false;
		AlphaBlendenable = true;
		srcblend = srcalpha;
		destblend = invsrccolor;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}




