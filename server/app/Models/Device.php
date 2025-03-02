<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Concerns\HasUuids;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Relations\HasMany;

class Device extends Model
{
    use HasUuids;

    protected $guarded = ['id'];

    public function measurements(): HasMany
    {
        return $this->hasMany(DeviceMeasurement::class);
    }
}
