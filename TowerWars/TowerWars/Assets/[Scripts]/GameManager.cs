﻿/*
Copyright 2017 Krzysztof Chobot

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.*/
using UnityEngine;

public class GameManager : MonoBehaviour {
    private static GameManager instance;
    public static GameManager Instance
    {
        get
        {
            if (instance == null)
                instance = FindObjectOfType<GameManager>();
            return instance;
        }
    }

    public static int TowerMaxCount = 20;
    public static int TowerCount = 0;
    public static int TowerProduced = 0;
    public static int ProjectilesCount = 0;

    public const int MapSize = 700;

    public const int TowerSize = 20;
    public const float PlaceForTowerSize = 40f;
}
