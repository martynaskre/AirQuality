<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Concerns\HasUuids;
use Illuminate\Database\Eloquent\Model;
use Illuminate\Database\Eloquent\Relations\BelongsTo;

class DeviceMeasurement extends Model
{
    use HasUuids;

    protected $guarded = ['id'];

    public function device(): BelongsTo
    {
        return $this->belongsTo(Device::class);
    }
}
