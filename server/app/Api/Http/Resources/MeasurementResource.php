<?php

namespace App\Api\Http\Resources;

use Illuminate\Http\Request;
use Illuminate\Http\Resources\Json\JsonResource;

class MeasurementResource extends JsonResource
{
    /**
     * Transform the resource into an array.
     *
     * @return array<string, mixed>
     */
    public function toArray(Request $request): array
    {
        return [
            'lux' => $this->lux,
            'temperature' => $this->temperature,
            'pressure' => $this->pressure,
            'humidity' => $this->humidity,
            'eco2' => $this->eco2,
            'tvoc' => $this->tvoc,
        ];
    }
}
