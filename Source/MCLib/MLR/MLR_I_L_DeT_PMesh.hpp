//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once
#define MLR_MLR_I_L_DeT_PMESH_HPP

#if !defined(MLR_MLR_HPP)
	#include <MLR\MLR.hpp>
#endif

#if !defined(MLR_MLR_I_C_DeT_PMESH_HPP)
	#include <MLR\MLR_I_C_DeT_PMesh.hpp>
#endif

namespace MidLevelRenderer {

	//##########################################################################
	//###### MLRIndexedPolyMesh with color and lighting two texture layer  #####
	//##########################################################################


	class MLR_I_L_DeT_PMesh:
		public MLR_I_C_DeT_PMesh
	{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Initialization
	//
	public:
		static void
			InitializeClass();
		static void
			TerminateClass();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Constructors/Destructors
	//
	protected:
		MLR_I_L_DeT_PMesh(
			ClassData *class_data,
			Stuff::MemoryStream *stream,
			int version
		);
		~MLR_I_L_DeT_PMesh();

	public:
		MLR_I_L_DeT_PMesh(ClassData *class_data=MLR_I_L_DeT_PMesh::DefaultData);

		static MLR_I_L_DeT_PMesh*
			Make(
				Stuff::MemoryStream *stream,
				int version
			);

		void
			Save(Stuff::MemoryStream *stream);

	public:
		virtual void
			SetNormalData(
				const Stuff::Vector3D *array,
				int point_count
			);
		virtual void
			GetNormalData(
				Stuff::Vector3D **array,
				int *point_count
			);

#if COLOR_AS_DWORD
		virtual void
			SetColorData(
				const DWORD *array,
				int point_count
			);
#else
		virtual void
			SetColorData(
				const Stuff::RGBAColor *array,
				int point_count
			);
#endif

		virtual void	Lighting(MLRLight* const*, int nrLights);

		virtual void
#if COLOR_AS_DWORD
			PaintMe(const DWORD *paintMe);
#else
			PaintMe(const Stuff::RGBAColor *paintMe);
#endif

		virtual int	
			TransformAndClip(Stuff::Matrix4D *, MLRClippingState, GOSVertexPool*,bool=false);

		virtual void
			TransformNoClip(Stuff::Matrix4D*, GOSVertexPool*,bool=false);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Class Data Support
	//
	public:
		static ClassData
			*DefaultData;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Testing
	//
	public:
		void
			TestInstance() const;

	virtual int
			GetSize()
		{ 
			Check_Object(this);
			int ret = MLR_I_C_DeT_PMesh::GetSize();
			ret += normals.GetSize();
			ret += litColors.GetSize();

			return ret;
		}

	protected:
		Stuff::DynamicArrayOf<Stuff::Vector3D> normals;		// Base address of normal list 

#if COLOR_AS_DWORD
		Stuff::DynamicArrayOf<DWORD> litColors;
		Stuff::DynamicArrayOf<DWORD> *actualColors;
#else
		Stuff::DynamicArrayOf<Stuff::RGBAColor> litColors;
		Stuff::DynamicArrayOf<Stuff::RGBAColor> *actualColors;
#endif

	};

//	MLR_I_L_DeT_PMesh*
//		CreateIndexedCube(Stuff::Scalar, Stuff::RGBAColor*, Stuff::Vector3D*, MLRState*);
	MLRShape*
		CreateIndexedIcosahedron_Color_Lit_DetTex(
			IcoInfo&,
			MLRState*,
			MLRState*
		);
}
