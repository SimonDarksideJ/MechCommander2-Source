//===========================================================================//
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
//===========================================================================//

#pragma once
#define MLR_MLR_I_C_DT_TMESH_HPP

#include <MLR\MLR.hpp>
#include <MLR\MLR_I_TMesh.hpp>
#include <MLR\MLR_I_C_PMesh.hpp>

namespace MidLevelRenderer {

	//##########################################################################
	//###### MLRIndexedTriMesh with color no lighting two texture layer  #######
	//##########################################################################


	class MLR_I_C_DT_TMesh:
		public MLR_I_DT_TMesh
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
		MLR_I_C_DT_TMesh(
			ClassData *class_data,
			Stuff::MemoryStream *stream,
			int version
		);
		~MLR_I_C_DT_TMesh();

	public:
		MLR_I_C_DT_TMesh(ClassData *class_data=MLR_I_C_DT_TMesh::DefaultData);

		static MLR_I_C_DT_TMesh*
			Make(
				Stuff::MemoryStream *stream,
				int version
			);

		void
			Save(Stuff::MemoryStream *stream);

	public:
#if COLOR_AS_DWORD
		virtual void
			SetColorData(
				const DWORD *array,
				int point_count
			);
		virtual void
			GetColorData(
				DWORD **array,
				int *point_count
			);
#else
		virtual void
			SetColorData(
				const Stuff::RGBAColor *array,
				int point_count
			);
		virtual void
			GetColorData(
				Stuff::RGBAColor **array,
				int *point_count
			);
#endif

		virtual void
#if COLOR_AS_DWORD
			PaintMe(const DWORD *paintMe);
#else
			PaintMe(const Stuff::RGBAColor *paintMe);
#endif

		virtual int	TransformAndClip(Stuff::Matrix4D *, MLRClippingState, GOSVertexPool*,bool=false);

		virtual void
			TransformNoClip(Stuff::Matrix4D*, GOSVertexPool*,bool=false);

		bool
			Copy(MLR_I_C_DT_PMesh*);

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
			int ret = MLR_I_DT_TMesh::GetSize();
			ret += colors.GetSize();

			return ret;
		}

	protected:
#if COLOR_AS_DWORD
		Stuff::DynamicArrayOf<DWORD> colors;	// Base address of color list 
		Stuff::DynamicArrayOf<DWORD> *actualColors;
#else
		Stuff::DynamicArrayOf<Stuff::RGBAColor> colors;	// Base address of color list 
		Stuff::DynamicArrayOf<Stuff::RGBAColor> *actualColors;
#endif
	};

	MLR_I_C_DT_TMesh*
		CreateIndexedTriCube_Color_NoLit_2Tex(Stuff::Scalar, MLRState*);
	MLRShape*
		CreateIndexedTriIcosahedron_Color_NoLit_2Tex(
			IcoInfo&,
			MLRState*,
			MLRState*
		);

}
