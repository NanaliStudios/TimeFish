//
//  FreeshTrailLayer.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 29..
//
//

#include "FreeshTrailLayer.h"


FreeshTrailLayer* FreeshTrailLayer::create(float fade, float minSeg, float stroke, const Color3B& color, const std::string& path)
{
    FreeshTrailLayer *ret = new (std::nothrow) FreeshTrailLayer();
    if (ret && ret->initWithFade(fade, minSeg, stroke, color, path))
    {
        //
        // set Start Opacity
        //
        ret->startOpacity = 125.0f;

        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void FreeshTrailLayer::update(float delta)
{
    if (!_startingPositionInitialized)
    {
        return;
    }
    
    delta *= _fadeDelta;
    
    unsigned int newIdx, newIdx2, i, i2;
    unsigned int mov = 0;
    
    // Update current points
    for(i = 0; i<_nuPoints; i++)
    {
        _pointState[i]-=delta;
        
        if(_pointState[i] <= 0)
            mov++;
        else
        {
            newIdx = i-mov;
            
            if(mov>0)
            {
                // Move data
                _pointState[newIdx] = _pointState[i];
                
                // Move point
                _pointVertexes[newIdx] = _pointVertexes[i];
                
                // Move vertices
                i2 = i*2;
                newIdx2 = newIdx*2;
                _vertices[newIdx2] = _vertices[i2];
                _vertices[newIdx2+1] = _vertices[i2+1];
                
                // Move color
                i2 *= 4;
                newIdx2 *= 4;
                _colorPointer[newIdx2+0] = _colorPointer[i2+0];
                _colorPointer[newIdx2+1] = _colorPointer[i2+1];
                _colorPointer[newIdx2+2] = _colorPointer[i2+2];
                _colorPointer[newIdx2+4] = _colorPointer[i2+4];
                _colorPointer[newIdx2+5] = _colorPointer[i2+5];
                _colorPointer[newIdx2+6] = _colorPointer[i2+6];
            }else
                newIdx2 = newIdx*8;
            
            const GLubyte op = (GLubyte)(_pointState[newIdx] * startOpacity);
            _colorPointer[newIdx2+3] = op;
            _colorPointer[newIdx2+7] = op;
        }
    }
    _nuPoints-=mov;
    
    // Append new point
    bool appendNewPoint = true;
    if(_nuPoints >= _maxPoints)
    {
        appendNewPoint = false;
    }
    
    else if(_nuPoints>0)
    {
        bool a1 = _pointVertexes[_nuPoints-1].getDistanceSq(_positionR) < _minSeg;
        bool a2 = (_nuPoints == 1) ? false : (_pointVertexes[_nuPoints-2].getDistanceSq(_positionR)< (_minSeg * 2.0f));
        if(a1 || a2)
        {
            appendNewPoint = false;
        }
    }
    
    if(appendNewPoint)
    {
        _pointVertexes[_nuPoints] = _positionR;
        _pointState[_nuPoints] = 1.0f;
        
        // Color assignment
        const unsigned int offset = _nuPoints*8;
        *((Color3B*)(_colorPointer + offset)) = _displayedColor;
        *((Color3B*)(_colorPointer + offset+4)) = _displayedColor;
        
        // Opacity
        _colorPointer[offset+3] = startOpacity;
        _colorPointer[offset+7] = startOpacity;
        
        // Generate polygon
        if(_nuPoints > 0 && _fastMode )
        {
            if(_nuPoints > 1)
            {
                ccVertexLineToPolygon(_pointVertexes, _stroke, _vertices, _nuPoints, 1);
            }
            else
            {
                ccVertexLineToPolygon(_pointVertexes, _stroke, _vertices, 0, 2);
            }
        }
        
        _nuPoints ++;
    }
    
    if( ! _fastMode )
    {
        ccVertexLineToPolygon(_pointVertexes, _stroke, _vertices, 0, _nuPoints);
    }
    
    // Updated Tex Coords only if they are different than previous step
    if( _nuPoints  && _previousNuPoints != _nuPoints ) {
        float texDelta = 1.0f / _nuPoints;
        for( i=0; i < _nuPoints; i++ ) {
            _texCoords[i*2] = Tex2F(0, texDelta*i);
            _texCoords[i*2+1] = Tex2F(1, texDelta*i);
        }
        
        _previousNuPoints = _nuPoints;
    }
}
