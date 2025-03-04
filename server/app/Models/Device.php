<?php

namespace App\Models;

use App\Enums\DeviceStatus;
use Carbon\Carbon;
use Illuminate\Database\Eloquent\Casts\Attribute;
use Illuminate\Database\Eloquent\Concerns\HasUuids;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Relations\HasMany;
use Illuminate\Database\Eloquent\Relations\HasOne;

class Device extends Model
{
    use HasUuids;

    protected $guarded = ['id'];

    protected function status(): Attribute
    {
        return Attribute::get(function () {
            $latestMeasurementDiff = $this->latestMeasurement->created_at->diffInMinutes(now());

            return match (true) {
                $latestMeasurementDiff <= 3 => DeviceStatus::Operational,
                $latestMeasurementDiff <= 10 => DeviceStatus::Delayed,
                default => DeviceStatus::Offline,
            };
        });
    }

    public function measurements(): HasMany
    {
        return $this->hasMany(DeviceMeasurement::class);
    }

    public function latestMeasurement(): HasOne
    {
        return $this->measurements()
            ->one()
            ->latestOfMany();
    }
}
