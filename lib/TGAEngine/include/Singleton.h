/*
 *  singleton.h
 *  Kore-Engine
 *
 *  Description: a basic singleton class
 *
 *  Created by Sean Chapel on 2/27/06.
 *  Copyright 2006 Seoushi Games. All rights reserved.
 *
 */

#ifndef SINGLETON
#define SINGLETON

namespace TGA
{
    template<class T> class Singleton
    {
        public:
            /// Get a pointer to the singleton
            static T* GetSingletonPtr()
            {
                static T m_Instance;
                return &m_Instance;
            }

        private:
            Singleton();
            ~Singleton();
            Singleton(Singleton const&);
            Singleton& operator=(Singleton const&);
    };
}

#endif
