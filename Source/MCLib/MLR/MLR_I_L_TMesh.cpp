//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#include "MLRHeaders.hpp"

#if defined(TRACE_ENABLED) && defined(MLR_TRACE)
	BitTrace *MLR_I_L_TMesh_Clip;
#endif

//#############################################################################
//###### MLRIndexedTriMesh with color but no lighting one texture layer  ######
//#############################################################################

MLR_I_L_TMesh::ClassData*
	MLR_I_L_TMesh::DefaultData = NULL;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
void
	MLR_I_L_TMesh::InitializeClass()
{
	Verify(!DefaultData);
	Verify(gos_GetCurrentHeap() == StaticHeap);
	DefaultData =
		new ClassData(
			MLR_I_L_TMeshClassID,
			"MidLevelRenderer::MLR_I_L_TMesh",
			MLR_I_C_TMesh::DefaultData,
			(MLRPrimitiveBase::Factory)&Make
		);
	Register_Object(DefaultData);

	#if defined(TRACE_ENABLED) && defined(MLR_TRACE)
		MLR_I_L_TMesh_Clip = new BitTrace("MLR_I_L_TMesh_Clip");
		Register_Object(MLR_I_L_TMesh_Clip);
	#endif
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
void
	MLR_I_L_TMesh::TerminateClass()
{
	Unregister_Object(DefaultData);
	delete DefaultData;
	DefaultData = NULL;

	#if defined(TRACE_ENABLED) && defined(MLR_TRACE)
		Unregister_Object(MLR_I_L_TMesh_Clip);
		delete MLR_I_L_TMesh_Clip;
	#endif
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
MLR_I_L_TMesh::MLR_I_L_TMesh(
	ClassData *class_data,
	MemoryStream *stream,
	int version
):
	MLR_I_C_TMesh(class_data, stream, version)
{
	Check_Pointer(this);
	Check_Pointer(stream);
	Verify(gos_GetCurrentHeap() == Heap);

	switch(version)
	{
		case 1:
		case 2:
		{
			STOP(("This class got created only after version 2 !"));
		}
		break;
		default:
		{
			MemoryStreamIO_Read(stream, &normals);
		}
		break;
	}

	litColors.SetLength(colors.GetLength());
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
MLR_I_L_TMesh::MLR_I_L_TMesh(ClassData *class_data):
	MLR_I_C_TMesh(class_data), normals(0)
{
	Check_Pointer(this);
	Verify(gos_GetCurrentHeap() == Heap);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
MLR_I_L_TMesh::~MLR_I_L_TMesh()
{
	Check_Object(this);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
MLR_I_L_TMesh*
	MLR_I_L_TMesh::Make(
		MemoryStream *stream,
		int version
	)
{
	Check_Object(stream);

	gos_PushCurrentHeap(Heap);
	MLR_I_L_TMesh *mesh = new MLR_I_L_TMesh(DefaultData, stream, version);
	gos_PopCurrentHeap();

	return mesh;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
void
	MLR_I_L_TMesh::Save(MemoryStream *stream)
{
	Check_Object(this);
	Check_Object(stream);

	MLR_I_C_TMesh::Save(stream);

	MemoryStreamIO_Write(stream, &normals);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
bool
	MLR_I_L_TMesh::Copy(MLR_I_L_PMesh *pMesh)
{
	Check_Pointer(this);
	Check_Object(pMesh);

	int len;
	Vector3D *_normals;

	MLR_I_C_TMesh::Copy(pMesh);

	pMesh->GetNormalData(&_normals, &len);
	SetNormalData(_normals, len);

	return true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
void
	MLR_I_L_TMesh::SetNormalData(
		const Vector3D *data,
		int dataSize
	)
{
	Check_Object(this); 
	Check_Pointer(data);

	Verify(coords.GetLength() == 0 || dataSize == coords.GetLength());
	Verify(colors.GetLength() == 0 || dataSize == colors.GetLength());
	Verify(texCoords.GetLength() == 0 || dataSize == texCoords.GetLength());

	normals.AssignData(data, dataSize);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
void
	MLR_I_L_TMesh::GetNormalData(
		Vector3D **data,
		int *dataSize
	)
{
	Check_Object(this); 
	*data = normals.GetData();
	*dataSize = normals.GetLength();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
void
	MLR_I_L_TMesh::SetColorData(
#if COLOR_AS_DWORD
		const DWORD *data,
#else
		const RGBAColor *data,
#endif
		int dataSize
	)
{
	Check_Object(this); 
	Check_Pointer(data);
	Verify(gos_GetCurrentHeap() == Heap);

	Verify(coords.GetLength() == 0 || dataSize == coords.GetLength());
	Verify(texCoords.GetLength() == 0 || dataSize == texCoords.GetLength());

	litColors.SetLength(dataSize);
	colors.AssignData(data, dataSize);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
void
	MLR_I_L_TMesh::PaintMe(
#if COLOR_AS_DWORD
		const DWORD *paintMe
#else
		const RGBAColor *paintMe
#endif
		
	)
{
	Check_Object(this);

#if 1
	Verify(colors.GetLength() == litColors.GetLength());
#else
	if(colors.GetLength() == litColors.GetLength())
	{
		litColors.SetLength(colors.GetLength());
	}
#endif

	int k, len = litColors.GetLength();

#if COLOR_AS_DWORD
	DWORD argb = GOSCopyColor(paintMe);

	for(k=0;k<len;k++)
	{
		litColors[k] = argb;
	}
#else
	for(k=0;k<len;k++)
	{
		litColors[k] = *paintMe;
	}
#endif
	// set the to use colors to the original colors ...
	// only lighting could overwrite this;
	actualColors = &litColors;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
void
	MLR_I_L_TMesh::TestInstance() const
{
	Verify(IsDerivedFrom(DefaultData));
}

extern DWORD gEnableTextureSort, gEnableAlphaSort;

#undef I_SAY_YES_TO_DUAL_TEXTURES
#define I_SAY_YES_TO_COLOR
#define I_SAY_YES_TO_LIGHTING

#define CLASSNAME MLR_I_L_TMesh

#if defined(TRACE_ENABLED) && defined(MLR_TRACE)
	#define SET_MLR_TMESH_CLIP() MLR_I_L_TMesh_Clip->Set()
	#define CLEAR_MLR_TMESH_CLIP() MLR_I_L_TMesh_Clip->Clear()
#else
	#define SET_MLR_TMESH_CLIP()
	#define CLEAR_MLR_TMESH_CLIP()
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	This include contains follwing functions:
//	void MLR_I_L_TMesh::TransformNoClip(Matrix4D*, GOSVertexPool*);
//	int MLR_I_L_TMesh::Clip(MLRClippingState, GOSVertexPool*);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <MLR\MLRTriangleClipping.hpp>

#undef I_SAY_YES_TO_COLOR
#undef I_SAY_YES_TO_LIGHTING

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	This include contains follwing functions:
//	void	Lighting (MLRLight**, int nrLights);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include <MLR\MLRTriangleLighting.hpp>
#undef CLASSNAME

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
MLRShape*
	MidLevelRenderer::CreateIndexedTriIcosahedron_Color_Lit(
		IcoInfo& icoInfo,
		MLRState *state
	)
{
	gos_PushCurrentHeap(Heap);
	MLRShape *ret = new MLRShape(20);
	Register_Object(ret);

	int i, j, k;
	long    nrTri = (long) ceil (icoInfo.all * pow (4.0f, icoInfo.depth));
	Point3D v[3];

	if(3*nrTri >= Limits::Max_Number_Vertices_Per_Mesh)
	{
		nrTri = Limits::Max_Number_Vertices_Per_Mesh/3;
	}

	Point3D *coords = new Point3D [nrTri*3];
	Register_Pointer(coords);
	
	Point3D *collapsedCoords = NULL;
	if(icoInfo.indexed==true)
	{
		collapsedCoords = new Point3D [nrTri*3];
		Register_Pointer(collapsedCoords);
	}

	unsigned short	*index = new unsigned short [nrTri*3];
	Register_Pointer(index);
	Vector2DScalar *texCoords = new Vector2DScalar[nrTri*3];
	Register_Pointer(texCoords);
	RGBAColor *colors = new RGBAColor[nrTri*3];
	Register_Pointer(colors);
	Vector3D *normals = new Vector3D[nrTri*3];
	Register_Pointer(normals);

	int uniquePoints = 0;
	for (k=0;k<20;k++)
	{
		triDrawn = 0;
		MLR_I_L_TMesh *mesh = new MLR_I_L_TMesh();
		Register_Object(mesh);

// setup vertex position information
	    for (j=0;j<3;j++)
		{
			v[j].x = vdata[tindices[k][j]][0];
			v[j].y = vdata[tindices[k][j]][1];
			v[j].z = vdata[tindices[k][j]][2];
		}
		subdivide (coords, v[0], v[1], v[2], icoInfo.depth, nrTri, icoInfo.radius);

		mesh->SetSubprimitiveLengths(NULL, nrTri);

		if(icoInfo.indexed==true)
		{
			uniquePoints = 1;
			collapsedCoords[0] = coords[0];
			index[0] = 0;

			for(i=1;i<nrTri*3;i++)
			{
				for(j=0;j<uniquePoints;j++)
				{
					if(coords[i] == collapsedCoords[j])
					{
						break;
					}
				}
				if(j==uniquePoints)
				{
					collapsedCoords[uniquePoints++] = coords[i];
				}
				index[i] = static_cast<unsigned short>(j);
			}
			mesh->SetCoordData(collapsedCoords, uniquePoints);
		}
		else
		{
			uniquePoints = nrTri*3;
			for(i=0;i<nrTri*3;i++)
			{
				index[i] = static_cast<unsigned short>(i);
			}
			mesh->SetCoordData(coords, nrTri*3);
		}

		mesh->SetIndexData(index, nrTri*3);

		mesh->FindFacePlanes();

		if(state == NULL)
		{
			for(i=0;i<uniquePoints;i++)
			{
				texCoords[i] = Vector2DScalar(0.0f, 0.0f);
			}
		}
		else
		{
			mesh->SetReferenceState(*state);
			if(state->GetTextureHandle() > 0)
			{
				if(icoInfo.indexed==true)
				{
					for(i=0;i<uniquePoints;i++)
					{
						texCoords[i] = 
							Vector2DScalar(
								(1.0f + collapsedCoords[i].x)/2.0f,
								(1.0f + collapsedCoords[i].y)/2.0f
							);
					}
				}
				else
				{
					for(i=0;i<nrTri;i++)
					{
						texCoords[3*i] = 
							Vector2DScalar(
								(1.0f + coords[3*i].x)/2.0f,
								(1.0f + coords[3*i].y)/2.0f
							);
						texCoords[3*i+1] = 
							Vector2DScalar(
								(1.0f + coords[3*i+1].x)/2.0f,
								(1.0f + coords[3*i+1].y)/2.0f
							);
						texCoords[3*i+2] = 
							Vector2DScalar(
								(1.0f + coords[3*i+2].x)/2.0f,
								(1.0f + coords[3*i+2].y)/2.0f
							);
					}
				}
			}
			else
			{
				for(i=0;i<uniquePoints;i++)
				{
					texCoords[i] = Vector2DScalar(0.0f, 0.0f);
				}
			}
		}
		mesh->SetTexCoordData(texCoords, uniquePoints);

		if(icoInfo.indexed==true)
		{
			for(i=0;i<uniquePoints;i++)
			{
				colors[i] = 
					RGBAColor(
						(1.0f + collapsedCoords[i].x)/2.0f,
						(1.0f + collapsedCoords[i].y)/2.0f,
						(1.0f + collapsedCoords[i].z)/2.0f,
						1.0f
					);
				normals[i].Normalize(collapsedCoords[i]);
			}
		}
		else
		{
			for(i=0;i<uniquePoints;i++)
			{
				colors[i] = 
					RGBAColor(
						(1.0f + coords[i].x)/2.0f,
						(1.0f + coords[i].y)/2.0f,
						(1.0f + coords[i].z)/2.0f,
						1.0f
					);
				normals[i].Normalize(coords[i]);
			}
		}
		mesh->SetColorData(colors, uniquePoints);
		mesh->SetNormalData(normals, uniquePoints);

		ret->Add(mesh);
		mesh->DetachReference();
	}

	Unregister_Pointer(normals);
	delete [] normals;

	Unregister_Pointer(colors);
	delete [] colors;

	Unregister_Pointer(texCoords);
	delete [] texCoords;

	Unregister_Pointer(index);
	delete [] index;

	if(icoInfo.indexed==true)
	{
		Unregister_Pointer(collapsedCoords);
		delete [] collapsedCoords;
	}
	
	Unregister_Pointer(coords);
	delete [] coords;

	gos_PopCurrentHeap();
	return ret;
}
