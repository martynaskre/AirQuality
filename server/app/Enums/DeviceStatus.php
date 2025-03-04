<?php

namespace App\Enums;

enum DeviceStatus: string
{
    case Operational = 'operational';
    case Delayed = 'delayed';
    case Offline = 'offline';
}
