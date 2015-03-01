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

#ifndef _LIQUID_FUN_OBJECT_H_
#define _LIQUID_FUN_OBJECT_H_

#ifndef _SPRITE_BASE_H_
#include "2d/core/SpriteBase.h"
#endif

//-----------------------------------------------------------------------------

class LiquidFunObject : public SpriteBase
{
private:
    typedef SpriteBase Parent;
    b2ParticleGroupDef mParticleGroupDef;
    b2ParticleGroup* mParticleGroup;
    S32 mShapeType;
    S32 mLiquidType;
    F32 mCircleRadius;
    F32 mParticleRadius;
    Vector2 mPolygonSize;
    bool mSolid;

public:

    enum shapeOptions
    {
        polygon = b2Shape::e_polygon,
        circle = b2Shape::e_circle
    };

    LiquidFunObject();
    virtual ~LiquidFunObject();

    static void initPersistFields();
    virtual void copyTo(SimObject* object);
    virtual void safeDelete( void );    
    virtual void sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer );

    void setCircleShape(F32 radius);
    void setPolygonShape(F32 width, F32 height);

    void setShapeType(const S32 shapeType) { mShapeType = shapeType; }
    inline S32 getShapeType() const { return mShapeType; }
    static const char* getShapeTypeDescription(const S32 shapeType);
    static S32 getShapeTypeEnum(const char* label);

    void setLiquidType(const S32 LiquidType) { mLiquidType = LiquidType; }
    inline S32 getLiquidType() const { return mLiquidType; }    
    static const char* getLiquidTypeDescription(const S32 LiquidType);
    static S32 getLiquidTypeEnum(const char* label);

    inline const Vector2& getPolygonSize() const { return mPolygonSize; }
    void setPolygonSize( const Vector2& size );

    inline F32 getCircleRadius() const { return mCircleRadius; }
    void setCircleRadius( const F32 radius );

    inline F32 getParticleRadius() const { return mParticleRadius; }
    void setParticleRadius( const F32 radius );

    virtual void setSolid( const bool solid )           { mSolid = solid; }
    inline bool getSolid(void) const                   { return mSolid; }

    /// Declare Console Object.
    DECLARE_CONOBJECT(LiquidFunObject);

protected:
    virtual void OnRegisterScene( Scene* pScene );
    virtual void OnUnregisterScene( Scene* pScene );    

    static const char* getShapeType(void* obj, const char* data) { return getShapeTypeDescription( static_cast<LiquidFunObject*>(obj)->getShapeType() ); }
    static bool        writeShapeType( void* obj, StringTableEntry pFieldName ) { return true; }
    static bool        setShapeType(void* obj, const char* data)
    {
        LiquidFunObject* pObject = static_cast<LiquidFunObject*>(obj);
        pObject->setShapeType(getShapeTypeEnum(data));
        return false;
    }

    static const char* getLiquidType(void* obj, const char* data) { return getLiquidTypeDescription( static_cast<LiquidFunObject*>(obj)->getLiquidType() ); }
    static bool        writeLiquidType( void* obj, StringTableEntry pFieldName ) { return true; }
    static bool        setLiquidType(void* obj, const char* data)
    {
        LiquidFunObject* pObject = static_cast<LiquidFunObject*>(obj);
        pObject->setLiquidType(getLiquidTypeEnum(data));
        return false;
    }

    static bool        writePolygonSize( void* obj, StringTableEntry pFieldName ) { return (static_cast<LiquidFunObject*>(obj)->getPolygonSize().notZero() && static_cast<LiquidFunObject*>(obj)->getShapeType() == shapeOptions::polygon); }
    static bool        writeCircleRadius( void* obj, StringTableEntry pFieldName ) { return (static_cast<LiquidFunObject*>(obj)->getCircleRadius() > 0.0f && static_cast<LiquidFunObject*>(obj)->getShapeType() == shapeOptions::circle); }
    static bool        writeParticleRadius( void* obj, StringTableEntry pFieldName ) { return static_cast<LiquidFunObject*>(obj)->getParticleRadius() > 0.0f; }

    static bool             setSolid(void* obj, const char* data)  { static_cast<LiquidFunObject*>(obj)->setSolid(dAtob(data)); return false; }
    static const char*      getSolid(void* obj, const char* data)  { return Con::getBoolArg( static_cast<LiquidFunObject*>(obj)->getSolid() ); }
    static bool             writeSolid( void* obj, StringTableEntry pFieldName ) { return static_cast<LiquidFunObject*>(obj)->getSolid() == true; }
};

#endif // _LIQUID_FUN_OBJECT_H_