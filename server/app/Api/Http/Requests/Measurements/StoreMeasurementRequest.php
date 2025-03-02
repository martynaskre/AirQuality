<?php

namespace App\Api\Http\Requests\Measurements;

use Illuminate\Contracts\Validation\ValidationRule;
use Illuminate\Foundation\Http\FormRequest;

class StoreMeasurementRequest extends FormRequest
{
    /**
     * Determine if the user is authorized to make this request.
     */
    public function authorize(): bool
    {
        return true;
    }

    /**
     * Get the validation rules that apply to the request.
     *
     * @return array<string, ValidationRule|array|string>
     */
    public function rules(): array
    {
        return [
            'proximity' => ['required', 'numeric'],
            'lux' => ['required', 'numeric'],
            'temperature' => ['required', 'numeric'],
            'pressure' => ['required', 'numeric'],
            'humidity' => ['required', 'numeric'],
            'eco2' => ['required', 'numeric'],
            'tvoc' => ['required', 'numeric'],
            'raw_h2' => ['required', 'numeric'],
            'raw_ethanol' => ['required', 'numeric'],
            'calibrated' => ['required', 'boolean'],
        ];
    }
}
