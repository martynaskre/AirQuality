<?php

namespace App\Console\Commands;

use App\Models\Device;
use Illuminate\Console\Command;
use Illuminate\Support\Str;

class DeviceProvision extends Command
{
    /**
     * The name and signature of the console command.
     *
     * @var string
     */
    protected $signature = 'device:provision';

    /**
     * The console command description.
     *
     * @var string
     */
    protected $description = 'Provisions new device.';

    /**
     * Execute the console command.
     */
    public function handle(): void
    {
        $identifier = strtoupper((string) Str::uuid());
        $pushKey = strtoupper((string) Str::uuid());

        Device::updateOrCreate([
            'identifier' => $identifier,
        ], [
            'push_key' => $pushKey,
        ]);

        $this->info('Device provisioned successfully.');
        $this->table(
            ['Serial Number', 'Push Key'],
            [[$identifier, $pushKey]]
        );
    }
}
