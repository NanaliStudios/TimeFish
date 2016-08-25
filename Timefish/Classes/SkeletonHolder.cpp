//
//  SkeletonHolder.cpp
//  Timefish
//
//  Created by midchow on 2015. 7. 30..
//
//

#include "SkeletonHolder.h"

static SkeletonHolder *_sharedSkeletonHolder = nullptr;

SkeletonHolder* SkeletonHolder::getInstance()
{
    if (!_sharedSkeletonHolder)
    {
        _sharedSkeletonHolder = new (std::nothrow) SkeletonHolder();
    }
    
    return _sharedSkeletonHolder;
}

void SkeletonHolder::initSkeletons()
{
    //
    // Freesh skeletons
    //
    {
        spAtlas* atlas = spAtlas_createFromFile("spines/fish/skeleton.atlas.txt", 0);
        spSkeletonJson* json = spSkeletonJson_create(atlas);
        
        //
        basicFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/basic/skeleton.json");
        CCASSERT(basicFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        etcFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/etc/skeleton.json");
        CCASSERT(etcFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        historyFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/history/skeleton.json");
        CCASSERT(historyFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        jobFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/job/skeleton.json");
        CCASSERT(jobFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        parodyFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/parody/skeleton.json");
        CCASSERT(parodyFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        retroFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/retro/skeleton.json");
        CCASSERT(retroFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        snapFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/snap/skeleton.json");
        CCASSERT(snapFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        specialFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/special/skeleton.json");
        CCASSERT(specialFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");

        //
        goldFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/GoldFish/skeleton.json");
        CCASSERT(goldFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");

        //
        spSkeletonJson_dispose(json);
    }
    
    //
    // Color Capsule/ Opstacle/ Light/ Timon/ Gift
    //
    {
        spAtlas* atlas = spAtlas_createFromFile("spines/play/skeleton.atlas.txt", 0);
        spSkeletonJson* json = spSkeletonJson_create(atlas);
        
        //
        capsuleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/play/colorball_boost/skeleton.json");
        CCASSERT(capsuleSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        obstacleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/play/coin_trap/skeleton.json");
        CCASSERT(obstacleSkeleton, json->error ? json->error : "Error reading skeleton data.");

        //
        fingerSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/play/finger/skeleton.json");
        CCASSERT(fingerSkeleton, json->error ? json->error : "Error reading skeleton data.");

        //
        shieldSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/play/shield/skeleton.json");
        CCASSERT(shieldSkeleton, json->error ? json->error : "Error reading skeleton data.");

        lightSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/play/round_light/skeleton.json");
        CCASSERT(lightSkeleton, json->error ? json->error : "Error reading skeleton data.");

        timonSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/play/timon/skeleton.json");
        CCASSERT(timonSkeleton, json->error ? json->error : "Error reading skeleton data.");

        giftSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/play/GiftBox/skeleton.json");
        CCASSERT(giftSkeleton, json->error ? json->error : "Error reading skeleton data.");

        //
        spSkeletonJson_dispose(json);
    }
    
    //
    // Draw
    //
    {
        spAtlas* atlas = spAtlas_createFromFile("spines/Vending Machine/skeleton.atlas.txt", 0);
        spSkeletonJson* json = spSkeletonJson_create(atlas);
        
        //
        drawSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/Vending Machine/skeleton.json");
        CCASSERT(drawSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        spSkeletonJson_dispose(json);
    }

    //
    // Idle Freesh skeletons
    //
    {
        spAtlas* atlas = spAtlas_createFromFile("spines/fish/skeleton.atlas.txt", 0);
        spSkeletonJson* json = spSkeletonJson_create(atlas);
        
        //
        randomFreeshSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/random_idle/skeleton.json");
        CCASSERT(randomFreeshSkeleton, json->error ? json->error : "Error reading skeleton data.");

        //
        basicFreeshIdleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/basic_idle/skeleton.json");
        CCASSERT(basicFreeshIdleSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        etcFreeshIdleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/etc_idle/skeleton.json");
        CCASSERT(etcFreeshIdleSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        historyFreeshIdleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/history_idle/skeleton.json");
        CCASSERT(historyFreeshIdleSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        jobFreeshIdleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/job_idle/skeleton.json");
        CCASSERT(jobFreeshIdleSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        parodyFreeshIdleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/parody_idle/skeleton.json");
        CCASSERT(parodyFreeshIdleSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        retroFreeshIdleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/retro_idle/skeleton.json");
        CCASSERT(retroFreeshIdleSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        snapFreeshIdleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/snap_idle/skeleton.json");
        CCASSERT(snapFreeshIdleSkeleton, json->error ? json->error : "Error reading skeleton data.");
        
        //
        specialFreeshIdleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/special_idle/skeleton.json");
        CCASSERT(specialFreeshIdleSkeleton, json->error ? json->error : "Error reading skeleton data.");

        //
        goldFreeshIdleSkeleton = spSkeletonJson_readSkeletonDataFile(json, "spines/fish/GoldFish_idle/skeleton.json");
        CCASSERT(goldFreeshIdleSkeleton, json->error ? json->error : "Error reading skeleton data.");

        //
        spSkeletonJson_dispose(json);
    }
}

SkeletonHolder::~SkeletonHolder()
{
    //
    spSkeletonData_dispose(basicFreeshSkeleton);
    spSkeletonData_dispose(etcFreeshSkeleton);
    spSkeletonData_dispose(historyFreeshSkeleton);
    spSkeletonData_dispose(jobFreeshSkeleton);
    spSkeletonData_dispose(parodyFreeshSkeleton);
    spSkeletonData_dispose(retroFreeshSkeleton);
    spSkeletonData_dispose(snapFreeshSkeleton);
    spSkeletonData_dispose(specialFreeshSkeleton);
    spSkeletonData_dispose(goldFreeshSkeleton);

    //
    spSkeletonData_dispose(capsuleSkeleton);
    spSkeletonData_dispose(obstacleSkeleton);
    spSkeletonData_dispose(fingerSkeleton);
    spSkeletonData_dispose(shieldSkeleton);
    spSkeletonData_dispose(lightSkeleton);
    spSkeletonData_dispose(timonSkeleton);

    //
    spSkeletonData_dispose(randomFreeshSkeleton);
    spSkeletonData_dispose(basicFreeshIdleSkeleton);
    spSkeletonData_dispose(etcFreeshIdleSkeleton);
    spSkeletonData_dispose(historyFreeshIdleSkeleton);
    spSkeletonData_dispose(jobFreeshIdleSkeleton);
    spSkeletonData_dispose(parodyFreeshIdleSkeleton);
    spSkeletonData_dispose(retroFreeshIdleSkeleton);
    spSkeletonData_dispose(snapFreeshIdleSkeleton);
    spSkeletonData_dispose(specialFreeshIdleSkeleton);
    spSkeletonData_dispose(goldFreeshIdleSkeleton);
}

spSkeletonData* SkeletonHolder::getSkeletonBySkinType(FreeshType skinType) {
    spSkeletonData *currSkeleton;
    switch (skinType) {
        case FreeshTypeBasic:
            currSkeleton = basicFreeshSkeleton;
            break;
        case FreeshTypeSnapBack:
            currSkeleton = snapFreeshSkeleton;
            break;
        case FreeshTypeJob:
            currSkeleton = jobFreeshSkeleton;
            break;
        case FreeshTypeRetro:
        case FreeshTypeRetroHistory:
        case FreeshTypeRetroHidden:
            currSkeleton = retroFreeshSkeleton;
            break;
        case FreeshTypeHistory:
        case FreeshTypeHistoryHidden:
            currSkeleton = historyFreeshSkeleton;
            break;
        case FreeshTypeEtc:
            currSkeleton = etcFreeshSkeleton;
            break;
        case FreeshTypeParody:
        case FreeshTypeParodyHidden:
            currSkeleton = parodyFreeshSkeleton;
            break;
        case FreeshTypeSpecial:
        case FreeshTypeSpecialHidden:
            currSkeleton = specialFreeshSkeleton;
            break;
        case FreeshTypeGoldfish:
            currSkeleton = goldFreeshSkeleton;
            break;
        default:
            break;
    }
    
    return currSkeleton;
}

spSkeletonData* SkeletonHolder::getIdelSkeletonBySkinType(FreeshType skinType) {
    spSkeletonData *currSkeleton;
    switch (skinType) {
        case FreeshTypeBasic:
            currSkeleton = basicFreeshIdleSkeleton;
            break;
        case FreeshTypeSnapBack:
            currSkeleton = snapFreeshIdleSkeleton;
            break;
        case FreeshTypeJob:
            currSkeleton = jobFreeshIdleSkeleton;
            break;
        case FreeshTypeRetro:
        case FreeshTypeRetroHistory:
        case FreeshTypeRetroHidden:
            currSkeleton = retroFreeshIdleSkeleton;
            break;
        case FreeshTypeHistory:
        case FreeshTypeHistoryHidden:
            currSkeleton = historyFreeshIdleSkeleton;
            break;
        case FreeshTypeEtc:
            currSkeleton = etcFreeshIdleSkeleton;
            break;
        case FreeshTypeParody:
        case FreeshTypeParodyHidden:
            currSkeleton = parodyFreeshIdleSkeleton;
            break;
        case FreeshTypeSpecial:
        case FreeshTypeSpecialHidden:
            currSkeleton = specialFreeshIdleSkeleton;
            break;
        case FreeshTypeGoldfish:
            currSkeleton = goldFreeshIdleSkeleton;
            break;
        default:
            break;
    }
    
    return currSkeleton;
}
