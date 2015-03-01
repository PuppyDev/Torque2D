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

function LiquidFunToy::create( %this )
{
    // Set the sandbox drag mode availability.
    Sandbox.allowManipulation( pan );
    Sandbox.allowManipulation( pull );
    
    // Set the manipulation mode.
    Sandbox.useManipulation( pull );
    
    // Toy properties
    LiquidFunToy.Shape = "Polygon";
    LiquidFunToy.ParticleRadius = 0.15;
    LiquidFunToy.VolumeSize = 6;
    LiquidFunToy.Solid = false;
    LiquidFunToy.LiquidType = "WaterParticle";
    LiquidFunToy.maxDebris = 0;
    
    // Add configuration option.
    %particleTypes = "WaterParticle,ZombieParticle,WallParticle,SpringParticle,ElasticParticle,";
    %particleTypes = %particleTypes @ "ViscousParticle,PowderParticle,TensileParticle,BarrierParticle,";
    %particleTypes = %particleTypes @ "ReactiveParticle,StaticPressureParticle,RepulsiveParticle";
    
    addSelectionOption("Circle,Polygon", "Set Shape", 2, "setShape", true, "Initial fluid area shape");
    addSelectionOption("Sand,Snow,Pebble", "Set size", 3, "setSize", true, "Set fluid particle size");
    addNumericOption("Volume", 3, 10, 1, "setVolume", 6, true, "Set the amount of particles by volume");
    addFlagOption("Solid Liquid", "setSolidLiquid", false, true, "Prevents other bodies from penetrating the particle group." );
    addSelectionOption(%particleTypes, "Set liquid type", 3, "setLiquidType", true, "Set fluid type");    
    addNumericOption("Amount of Debris", 0, 30, 1, "setMaxDebris", LiquidFunToy.maxDebris, true, "Sets the amount of debris created.");
    
    // Reset the toy.
    LiquidFunToy.reset();    
}

//-----------------------------------------------------------------------------

function LiquidFunToy::setSolidLiquid(%this, %value)
{
    LiquidFunToy.Solid = %value;
}

//-----------------------------------------------------------------------------

function LiquidFunToy::setLiquidType(%this, %value)
{
    LiquidFunToy.LiquidType = %value;    
}

//-----------------------------------------------------------------------------

function LiquidFunToy::setShape(%this, %value)
{
    LiquidFunToy.Shape = %value;    
}

//-----------------------------------------------------------------------------

function LiquidFunToy::setSize(%this, %value)
{
    switch$(%value)
    {
        case "Sand":
            LiquidFunToy.ParticleRadius = 0.08;
        case "Snow":
            LiquidFunToy.ParticleRadius = 0.15;
        case "Pebble":
            LiquidFunToy.ParticleRadius = 0.2;
    }    
}

//-----------------------------------------------------------------------------

function LiquidFunToy::setVolume(%this, %value)
{
    LiquidFunToy.VolumeSize = %value;
}

//-----------------------------------------------------------------------------

function LiquidFunToy::destroy( %this )
{
}

//-----------------------------------------------------------------------------

function LiquidFunToy::reset( %this )
{
    // Clear the scene.
    SandboxScene.clear();

    SandboxWindow.setCameraSize( 50, 37.5 );
    
    SandboxScene.setGravity( 0, -9.8 );

    // Create the ground.
    %this.createGround();
               
    // Create background.
    %this.createBackground();
    
    // Create liquid
    %this.createLiquidFun();
    
    // Create debris
    %this.schedule(3000, createDebris);
}

//-----------------------------------------------------------------------------

function LiquidFunToy::setMaxDebris(%this, %value)
{
    %this.maxDebris = %value;
}

//-----------------------------------------------------------------------------

function LiquidFunToy::createDebris(%this)
{
    for (%image = 0; %i < LiquidFunToy.maxDebris; %i++)
    {
        %randomPosition = getRandom(-10, 10) SPC getRandom(2, 8);
        
        %obj = new Sprite();   
        
        %obj.setImage("ToyAssets:crate");
        %obj.setPosition(%randomPosition);
        %obj.setSize(1.5, 1.5);
        %obj.setDefaultFriction(1.0);
        %obj.setDefaultDensity(1);
        %obj.createPolygonBoxCollisionShape(1.4, 1.4);
        %obj.setBullet( true );
        
        SandboxScene.add(%obj);
    }
}

//-----------------------------------------------------------------------------

function LiquidFunToy::createGround( %this )
{
    // Create the ground
    %ground = new Scroller();
    %ground.setBodyType("static");
    %ground.Image = "ToyAssets:woodGround";
    %ground.setPosition(0, -16);
    %ground.setSize(80, 6);
    %ground.createEdgeCollisionShape(80/-2, 3, 80/2, 3);
    %ground.setRepeatX(80 / 30);    
    SandboxScene.add(%ground);
        
    %wall = new Scroller();
    %wall.setBodyType("static");
    %wall.Image = "ToyAssets:tiles";
    %wall.Frame = "12";
    %wall.setPosition(-15, -0.5);
    %wall.setSize(6, 25);
    %wall.createEdgeCollisionShape(3, 25/-2, 3, 25/2);
    %wall.setRepeatY(3); 
    SandboxScene.add(%wall);  
    
    %wall = new Scroller();
    %wall.setBodyType("static");
    %wall.Image = "ToyAssets:tiles";
    %wall.Frame = "12";
    %wall.setPosition(15, -0.5);
    %wall.setSize(6, 25);    
    %wall.createEdgeCollisionShape(-3, 25/-2, -3, 25/2);
    %wall.setRepeatY(3); 
    SandboxScene.add(%wall);  
}

//-----------------------------------------------------------------------------

function LiquidFunToy::createBackground( %this )
{    
    // Create the sprite.
    %object = new Sprite();
    
    // Set the sprite as "static" so it is not affected by gravity.
    %object.setBodyType( static );
       
    // Always try to configure a scene-object prior to adding it to a scene for best performance.

    // Set the position.
    %object.Position = "0 0";

    // Set the size.        
    %object.Size = "100 75";
    
    // Set to the furthest background layer.
    %object.SceneLayer = 31;
    
    // Set the scroller to use an animation!
    %object.Image = "ToyAssets:highlightBackground";
    
    // Set the blend color.
    %object.BlendColor = SlateGray;
            
    // Add the sprite to the scene.
    SandboxScene.add( %object );    
}

//-----------------------------------------------------------------------------

function LiquidFunToy::createLiquidFun( %this )
{
    %object = "";
    
    if (LiquidFunToy.Shape $= "Polygon")
    {
        %object = new LiquidFunObject()
        {
            ParticleRadius = LiquidFunToy.ParticleRadius;
            ShapeType = LiquidFunToy.Shape;
            PolygonSize = LiquidFunToy.VolumeSize;
            Size = LiquidFunToy.VolumeSize;
            Solid = LiquidFunToy.Solid;
            LiquidType = LiquidFunToy.LiquidType;
        };
    }
    else
    {
        %object = new LiquidFunObject()
        {
            ParticleRadius = LiquidFunToy.ParticleRadius;
            ShapeType = LiquidFunToy.Shape;
            CircleRadius = LiquidFunToy.VolumeSize;
            Size = LiquidFunToy.VolumeSize;
            Solid = LiquidFunToy.Solid;
            LiquidType = LiquidFunToy.LiquidType;
        };
    }
        
    // Set the position.
    %object.Position = "0 0";
    %object.setBodyType( static );
    
    // Set the sprite to use an image.  This is known as "static" image mode.
    %object.Image = "ToyAssets:Tiles";
    
    // We don't really need to do this as the frame is set to zero by default.
    %object.Frame = 0;
    
    SandboxScene.add( %object );    
}