<?php

namespace App\Api\Http\Controllers;

use App\Api\Http\Resources\DeviceResource;
use App\Models\Device;
use Illuminate\Http\Request;

class DeviceController extends Controller
{
    public function show(Request $request): DeviceResource
    {
        /* @var $device Device */
        $device = $request->get('device');

        return new DeviceResource($device);
    }
}
