<?php

namespace Database\Seeders;

use App\Models\Device;
use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

class DeviceSeeder extends Seeder
{
    private array $devices = [
        '646974F7-0EBC-4207-8403-9E551B1FE601' => [
            'push_key' => '47402C22-65EE-434A-850F-561A8E6AC410',
        ]
    ];

    public function run(): void
    {
        foreach ($this->devices as $identifier => $device) {
            Device::updateOrCreate([
                'identifier' => $identifier,
            ], $device);
        }
    }
}
