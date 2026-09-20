shader_type canvas_item;

uniform vec3 key_color : source_color = vec3(0.0, 1.0, 0.0);

uniform float similarity : hint_range(0.0, 1.0) = 0.12;
uniform float smoothness : hint_range(0.0, 0.5) = 0.08;
uniform float despill : hint_range(0.0, 1.0) = 1.0;

vec2 get_chroma(vec3 color) {
    float cb = -0.168736 * color.r - 0.331264 * color.g + 0.5 * color.b;
    float cr =  0.5 * color.r - 0.418688 * color.g - 0.081312 * color.b;

    return vec2(cb, cr);
}

void fragment() {
    vec4 color = texture(TEXTURE, UV);

    vec2 pixel_chroma = get_chroma(color.rgb);
    vec2 key_chroma = get_chroma(key_color);

    float chroma_distance = distance(pixel_chroma, key_chroma);

    float alpha = smoothstep(
        similarity,
        similarity + smoothness,
        chroma_distance
    );

    float green_excess = max(
        color.g - max(color.r, color.b),
        0.0
    );

    color.g -= green_excess * despill;

    COLOR = vec4(color.rgb, color.a * alpha);
}