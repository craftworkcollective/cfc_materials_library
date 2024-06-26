//
//  AtlasManager.h
//
//  Created by Nicole Yi Messier
//

#pragma once


class AtlasManager {
  public:
    static AtlasManager &get()
    {
        static AtlasManager sInstance;
        return sInstance;
    }

    AtlasManager(){};
    ~AtlasManager(){};

    void setup();

};

