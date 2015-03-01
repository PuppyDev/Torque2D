//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "2d/sceneobject/liquidFunObject.h"

// Script bindings.
#include "2d/sceneobject/liquidFunObject_ScriptBinding.h"

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(LiquidFunObject);

//------------------------------------------------------------------------------

LiquidFunObject::LiquidFunObject()
{        
    mSolid = false;
    mParticleRadius = 1.0f;
    mLiquidType = b2_waterParticle;
}

//------------------------------------------------------------------------------

LiquidFunObject::~LiquidFunObject()
{    
}

//------------------------------------------------------------------------------

static EnumTable::Enums shapeTypeEnum[] =
{
    { LiquidFunObject::polygon, "Polygon"  },
    { LiquidFunObject::circle,"Circle" }   
};
static EnumTable shapeTypeTable(2, &shapeTypeEnum[0]);


S32 LiquidFunObject::getShapeTypeEnum(const char* label)
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(shapeTypeEnum) / sizeof(EnumTable::Enums)); i++)
    {
        if( dStricmp(shapeTypeEnum[i].label, label) == 0)
            return (S32)shapeTypeEnum[i].index;
    }

    // Warn.
    Con::warnf("LiquidFunObject::shapeTypeEnum() - Invalid shape type of '%s'", label );

    return (S32)-1;
}

//-----------------------------------------------------------------------------

const char* LiquidFunObject::getShapeTypeDescription(S32 shapeType)
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(shapeTypeEnum) / sizeof(EnumTable::Enums)); i++)
    {
        if( shapeTypeEnum[i].index == shapeType )
            return shapeTypeEnum[i].label;
    }

    // Warn.
    Con::warnf( "LiquidFunObject::getShapeTypeDescription() - Invalid shape type." );

    return StringTable->EmptyString;
}

//-----------------------------------------------------------------------------

static EnumTable::Enums liquidParticleTypes[] =
{
    { b2ParticleFlag::b2_waterParticle, "WaterParticle" },
    { b2ParticleFlag::b2_zombieParticle, "ZombieParticle" },
    { b2ParticleFlag::b2_wallParticle, "WallParticle" },
    { b2ParticleFlag::b2_springParticle, "SpringParticle" },
    { b2ParticleFlag::b2_elasticParticle, "ElasticParticle" },
    { b2ParticleFlag::b2_viscousParticle, "ViscousParticle" },
    { b2ParticleFlag::b2_powderParticle, "PowderParticle" },
    { b2ParticleFlag::b2_tensileParticle, "TensileParticle" },
    { b2ParticleFlag::b2_colorMixingParticle, "ColorMixingParticle" },
    { b2ParticleFlag::b2_destructionListenerParticle, "DestructionListenerParticle" },
    { b2ParticleFlag::b2_barrierParticle, "BarrierParticle" },
    { b2ParticleFlag::b2_staticPressureParticle, "StaticPressureParticle" },
    { b2ParticleFlag::b2_reactiveParticle, "ReactiveParticle" },
    { b2ParticleFlag::b2_repulsiveParticle, "RepulsiveParticle" }
};
static EnumTable liquidParticleTable(14, &liquidParticleTypes[0]);

//-----------------------------------------------------------------------------

S32 LiquidFunObject::getLiquidTypeEnum(const char* label)
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(liquidParticleTypes) / sizeof(EnumTable::Enums)); i++)
    {
        if( dStricmp(liquidParticleTypes[i].label, label) == 0)
            return (S32)liquidParticleTypes[i].index;
    }

    // Warn.
    Con::warnf("LiquidFunObject::getLiquidTypeEnum() - Invalid shape type of '%s'", label );

    return (S32)-1;
}

//-----------------------------------------------------------------------------

const char* LiquidFunObject::getLiquidTypeDescription(S32 shapeType)
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(liquidParticleTypes) / sizeof(EnumTable::Enums)); i++)
    {
        if( liquidParticleTypes[i].index == shapeType )
            return liquidParticleTypes[i].label;
    }

    // Warn.
    Con::warnf( "LiquidFunObject::getLiquidTypeDescription() - Invalid shape type." );

    return StringTable->EmptyString;
}

//-----------------------------------------------------------------------------

void LiquidFunObject::initPersistFields()
{
    // Call parent.
    Parent::initPersistFields();    

    addProtectedField("ShapeType", TypeEnum, NULL, &setShapeType, &getShapeType, &writeShapeType, 1, &shapeTypeTable, "" );
    addProtectedField("LiquidType", TypeEnum, NULL, &setLiquidType, &getLiquidType, &writeLiquidType, 1, &liquidParticleTable, "" );    
    addProtectedField("Solid", TypeBool, NULL, &setSolid, &getSolid, &writeSolid, "");
    addField("CircleRadius", TypeF32, Offset(mCircleRadius, LiquidFunObject), &writeCircleRadius, "");
    addField("ParticleRadius", TypeF32, Offset(mParticleRadius, LiquidFunObject), &writeParticleRadius, "");
    addField("PolygonSize", TypeVector2, Offset(mPolygonSize, LiquidFunObject), &writePolygonSize, "");
}

//------------------------------------------------------------------------------

void LiquidFunObject::copyTo(SimObject* object)
{
    // Fetch particle asset object.
   LiquidFunObject* pLiquidFunObject = static_cast<LiquidFunObject*>( object );

   // Sanity!
   AssertFatal( pLiquidFunObject != NULL, "LiquidFunObject::copyTo() - Object is not the correct type.");

   pLiquidFunObject->setShapeType( getShapeType() );

   if (getShapeType() == shapeOptions::polygon)
       pLiquidFunObject->setPolygonSize( getPolygonSize() );
   else if (getShapeType() == shapeOptions::circle)
       pLiquidFunObject->setCircleRadius( getCircleRadius() );

   pLiquidFunObject->setParticleRadius( getParticleRadius() );
   pLiquidFunObject->setSolid( getSolid() );
   pLiquidFunObject->setLiquidType( getLiquidType() );

   // Copy parent.
   Parent::copyTo( object );   
}

//-----------------------------------------------------------------------------

void LiquidFunObject::safeDelete( void )
{    
    // Call parent which will deal with the deletion.
    Parent::safeDelete();
}

//-----------------------------------------------------------------------------

void LiquidFunObject::OnRegisterScene( Scene* pScene )
{
    // Call parent.
    Parent::OnRegisterScene( pScene );

    // Add always in scope.
    pScene->getWorldQuery()->addAlwaysInScope( this );
    
    b2PolygonShape polygonShape;
    b2CircleShape circleShape;
    b2Vec2 pos(getPosition().x, getPosition().y);
    if (mShapeType == shapeOptions::polygon)
    {        
        polygonShape.SetAsBox(mPolygonSize.x, mPolygonSize.y, pos, getAngle());       
        mParticleGroupDef.shape = &polygonShape;
    }
    else if (mShapeType == shapeOptions::circle)
    {        
        circleShape.m_radius = mCircleRadius;
        circleShape.m_p = pos;
        mParticleGroupDef.shape = &circleShape;
    }
    
    mParticleGroupDef.flags = getLiquidType();

    if (getSolid())
		mParticleGroupDef.groupFlags = b2_solidParticleGroup + ~b2_particleGroupCanBeEmpty;
	else
		mParticleGroupDef.groupFlags = ~b2_particleGroupCanBeEmpty;

    b2ParticleSystem* pSystem = pScene->GetParticleSystem();
    pSystem->SetRadius(getParticleRadius());
    pSystem->SetDamping(0.2f);
    pSystem->SetGravityScale(0.5f);
    mParticleGroup = pSystem->CreateParticleGroup(mParticleGroupDef);
}

//-----------------------------------------------------------------------------

void LiquidFunObject::OnUnregisterScene( Scene* pScene )
{    
    // Remove always in scope.
    pScene->getWorldQuery()->removeAlwaysInScope( this );

	// Stop	
	mParticleGroup->DestroyParticles();

    // Call parent.
    Parent::OnUnregisterScene( pScene );
}

//-----------------------------------------------------------------------------
float smoothstep(float x) { return x * x * (3 - 2 * x); }
void LiquidFunObject::sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer )
{
    // Finish if we can't render.
    S32 particleCount = mpScene->getWorld()->GetParticleSystemList()->GetParticleCount();

    if (!particleCount)
        return;
    
    // Flush.
    pBatchRenderer->flush( getScene()->getDebugStats().batchIsolatedFlush );

    F32 currentscale = 1.0;
    F32 radius = getScene()->getWorld()->GetParticleSystemList()->GetRadius();
    b2Vec2* centers = getScene()->getWorld()->GetParticleSystemList()->GetPositionBuffer();
    S32 count = getScene()->getWorld()->GetParticleSystemList()->GetParticleCount();
    static unsigned int particle_texture = 0;

	if (!particle_texture || !glIsTexture(particle_texture)) // Android deletes textures upon sleep etc.
	{
		// generate a "gaussian blob" texture procedurally
		glGenTextures(1, &particle_texture);
		b2Assert(particle_texture);
		const int TSIZE = 64;
		unsigned char tex[TSIZE][TSIZE][4];
		for (int y = 0; y < TSIZE; y++)
		{
			for (int x = 0; x < TSIZE; x++)
			{
				float fx = (x + 0.5f) / TSIZE * 2 - 1;
				float fy = (y + 0.5f) / TSIZE * 2 - 1;
				float dist = sqrtf(fx * fx + fy * fy);
				unsigned char intensity = (unsigned char)(dist <= 1 ? smoothstep(1 - dist) * 255 : 0);
				tex[y][x][0] = tex[y][x][1] = tex[y][x][2] = 128;
				tex[y][x][3] = intensity;
			}
		}
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, particle_texture);
		#ifdef __ANDROID__
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		#endif
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex);

		glDisable(GL_TEXTURE_2D);

		glEnable(GL_POINT_SMOOTH);
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, particle_texture);

	#ifdef __ANDROID__
		glEnable(GL_POINT_SPRITE_OES);
		glTexEnvf(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);
		const float particle_size_multiplier = 3;  // because of falloff
		const float global_alpha = 1;  // none, baked in texture
	#else
		/*
		// normally this is how we'd enable them on desktop OpenGL,
		// but for some reason this is not applying textures, so we use alpha instead
		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		*/
		const float particle_size_multiplier = 2;  // no falloff
		const float global_alpha = 0.35f;  // instead of texture
	#endif

	glPointSize(radius * currentscale * particle_size_multiplier);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &centers[0].x);
	
    /*if (colors)    
	{
		#ifndef __ANDROID__
			// hack to render with proper alpha on desktop for Testbed
			b2ParticleColor * mcolors = const_cast<b2ParticleColor *>(colors);
			for (int i = 0; i < count; i++)
			{
				mcolors[i].a = static_cast<uint8>(global_alpha * 255);
			}
		#endif
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[0].r);
	}
	else
	{*/
		glColor4f(1, 1, 1, global_alpha);
	//}

	glDrawArrays(GL_POINTS, 0, count);

	glDisableClientState(GL_VERTEX_ARRAY);
	//if (colors) glDisableClientState(GL_COLOR_ARRAY);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	#ifdef __ANDROID__
		glDisable(GL_POINT_SPRITE_OES);
	#endif
/*
    const b2Vec2* positionBuffer = mpScene->getWorld()->GetParticlePositionBuffer();
    F32 particleRadius = mpScene->getWorld()->GetParticleRadius();
    const int TSIZE = 64;

    for (U32 particleIndex = 0; particleIndex < particleCount; particleIndex++)
    {

    }

   
    
    // Fetch emitter count.
    const U32 emitterCount = mEmitters.size();

    // Render all the emitters.
    for ( U32 emitterIndex = 0; emitterIndex < emitterCount; ++emitterIndex )
    {
        // Fetch the emitter node.
        EmitterNode* pEmitterNode = mEmitters[emitterIndex];

        // Fetch the particle emitter.
        ParticleAssetEmitter* pParticleAssetEmitter = pEmitterNode->getAssetEmitter();

        // Skip if the emitter is not visible.
        if ( !pEmitterNode->getVisible() )
            continue;

        // Skip if there are no active particles.
        if ( !pEmitterNode->getActiveParticles() )
            continue;       

        // Fetch both image and animation assets.
        const AssetPtr<ImageAsset>& imageAsset = pParticleAssetEmitter->getImageAsset();
        const AssetPtr<AnimationAsset>& animationAsset = pParticleAssetEmitter->getAnimationAsset();

        // Fetch static mode.
        const bool isStaticFrameProvider = pParticleAssetEmitter->isStaticFrameProvider();

        // Are we in static mode?
        if ( isStaticFrameProvider )
        {
            // Yes, so skip if no image available.
            if ( imageAsset.isNull() )
                continue;
        }
        else
        {
            // No, so skip if no animation available.
            if ( animationAsset.isNull() )
                continue;
        }

        // Flush.
        pBatchRenderer->flush( getScene()->getDebugStats().batchIsolatedFlush );

        // Intense particles?
        if ( pParticleAssetEmitter->getIntenseParticles() )
        {
            // Yes, so set additive blending.
            pBatchRenderer->setBlendMode( GL_SRC_ALPHA, GL_ONE );
        }
        else
        {
            // No, so set standard blend options.
            if ( mBlendMode )
            {
                pBatchRenderer->setBlendMode( mSrcBlendFactor, mDstBlendFactor );
            }
            else
            {
                pBatchRenderer->setBlendOff();
            }
        }

        // Set alpha-testing.
        pBatchRenderer->setAlphaTestMode( pParticleAssetEmitter->getAlphaTest() );

        // Save the transformation.
        glPushMatrix();

        // Is the Position attached to the emitter?
        if ( pParticleAssetEmitter->getAttachPositionToEmitter() )
        {
            // Yes, so get player position.
            const Vector2 renderPosition = getRenderPosition();

            // Move into emitter-space.
            glTranslatef( renderPosition.x, renderPosition.y, 0.0f );

            // Is the rotation attached to the emitter?
            if ( pParticleAssetEmitter->getAttachRotationToEmitter() )
            {
                // Yes, so rotate into emitter-space.
                // NOTE:- We need clockwise rotation here.
                glRotatef( mRadToDeg(getRenderAngle()), 0.0f, 0.0f, 1.0f );
            }
        }

        // Fetch the oldest-in-front flag.
        const bool oldestInFront = pParticleAssetEmitter->getOldestInFront();

        // Fetch the starting particle (using appropriate particle order).
        ParticleSystem::ParticleNode* pParticleNode = oldestInFront ? pEmitterNode->getFirstParticle() : pEmitterNode->getLastParticle();

        // Fetch the particle node head.
        ParticleSystem::ParticleNode* pParticleNodeHead = pEmitterNode->getParticleNodeHead();

        // Process all particle nodes.
        while ( pParticleNode != pParticleNodeHead )
        {
            // Fetch the frame provider.
            const ImageFrameProviderCore& frameProvider = pParticleNode->mFrameProvider;

            // Fetch the frame area.
            const ImageAsset::FrameArea::TexelArea& texelFrameArea = frameProvider.getProviderImageFrameArea().mTexelArea;

            // Frame texture.
            TextureHandle& frameTexture = frameProvider.getProviderTexture();

            // Fetch the particle render OOBB.
            Vector2* renderOOBB = pParticleNode->mRenderOOBB;

            // Fetch lower/upper texture coordinates.
            const Vector2& texLower = texelFrameArea.mTexelLower;
            const Vector2& texUpper = texelFrameArea.mTexelUpper;

            // Submit batched quad.
            pBatchRenderer->SubmitQuad(
                renderOOBB[0],
                renderOOBB[1],
                renderOOBB[2],
                renderOOBB[3],
                Vector2( texLower.x, texUpper.y ),
                Vector2( texUpper.x, texUpper.y ),
                Vector2( texUpper.x, texLower.y ),
                Vector2( texLower.x, texLower.y ),
                frameTexture,
                pParticleNode->mColor );

            // Move to next Particle ( using appropriate sort-order ).
            pParticleNode = oldestInFront ? pParticleNode->mNextNode : pParticleNode->mPreviousNode;
        };

        // Flush.
        pBatchRenderer->flush( getScene()->getDebugStats().batchIsolatedFlush );

        // Restore the transformation.
        glPopMatrix();
    }*/
}

//-----------------------------------------------------------------------------

void LiquidFunObject::setCircleShape(F32 radius)
{
    b2CircleShape shape;
    shape.m_radius = radius;
    mParticleGroupDef.shape = &shape;
}

//-----------------------------------------------------------------------------

void LiquidFunObject::setPolygonShape(F32 width, F32 height)
{
    b2PolygonShape shape;
    shape.SetAsBox(width, height);
    mParticleGroupDef.shape = &shape;
}

//-----------------------------------------------------------------------------

void LiquidFunObject::setPolygonSize( const Vector2& size )
{
    // Debug Profiling.
    PROFILE_SCOPE(LiquidFunObject_SetPolygonSize);

    mPolygonSize = size;
    mShapeType = shapeOptions::polygon;
}

//-----------------------------------------------------------------------------

void LiquidFunObject::setCircleRadius( const F32 radius )
{
    // Debug Profiling.
    PROFILE_SCOPE(LiquidFunObject_SetCircleRadius);

    mCircleRadius = radius;
    mShapeType = shapeOptions::circle;
}

//-----------------------------------------------------------------------------

void LiquidFunObject::setParticleRadius( const F32 radius )
{
    // Debug Profiling.
    PROFILE_SCOPE(LiquidFunObject_SetParticleRadius);

    mParticleRadius = radius;    
}