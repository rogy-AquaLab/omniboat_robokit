#!/usr/bin/env bash

set -eu

[ $# -gt 1 ] && [ "$1" = "supports" ] && {
    echo $@ >&2
    exit 0
}

image_table='
https://drive.google.com/uc?export=view&id=1NnLZCr2QoSQeOK8d_1CTcOu5fGkIJJQ4 IMG_3667.JPG
https://drive.google.com/uc?export=view&id=12ME7Xeg_XfVyJiwRXnjB20jwG0ZKBmt8 IMG_3635.JPG
https://drive.google.com/uc?export=view&id=1PRk7nVO3amvb6OxNI0-vz9hpQDxor_4o IMG_3668.JPG
https://drive.google.com/uc?export=view&id=1gsGabJ_zs99jCpi-WIGXs_zz9pFPm9S5 IMG_0021.JPG
https://drive.google.com/uc?export=view&id=16pc9SEHGkV5IHctwzKgKhw27AfOJ3xWB IMG_0022.JPG
https://drive.google.com/uc?export=view&id=1ff9NKm6znETd7WJIL7DMcZ3lYKo9_XHs IMG_0023.JPG
https://drive.google.com/uc?export=view&id=1AS6mrPOncnSq8a68yxQnwwvNgj-GkZCx IMG_3689.JPG
https://drive.google.com/uc?export=view&id=1UgqDDgb4ttHz66kYFX5iZ0lHqvzK-8_Q IMG_3690.JPG
https://drive.google.com/uc?export=view&id=1EbXZl0ilSZsssofCAOuk0Aeox8csUG3R IMG_3691.JPG
https://drive.google.com/uc?export=view&id=1yY1cP0qCtzZgHZ32mx-YapXnAIUQyrBj IMG_3692.JPG
https://drive.google.com/uc?export=view&id=1xQ9frnsAfyMa-Yuo8UUKy-Dsj6gyi7Qi IMG_3693.JPG
https://drive.google.com/uc?export=view&id=1Atr6WnDFc-E5m13HT9SrVj9lmlojRefx IMG_3665.JPG
https://drive.google.com/uc?export=view&id=1uxaMr3Wk7R9FYDjT7tKY_b6meesPuVix IMG_3694.JPG
https://drive.google.com/uc?export=view&id=1bkWYkLBfxFbxwv7rj-aDdUspC8WE2wiO IMG_3697.JPG
https://drive.google.com/uc?export=view&id=1kudI_gUDvmazpXuXopX83mZ0ZC6LIms5 IMG_3698.JPG
https://drive.google.com/uc?export=view&id=1eUluc42uEhR5JCbpzReUKo8A7LqOKYme IMG_3699.JPG
https://drive.google.com/uc?export=view&id=1dbhbVPmocNYxuC6dgz-IaoDzoRTzy8Mb omazinai.jpg
https://drive.google.com/uc?export=view&id=1pI5aElj6qVAAyscx3fNOTEJwPrkgFAL8 IMG_3701.JPG
https://drive.google.com/uc?export=view&id=1BxwFDRLr0_ka-6UUgtQLugg2pJYhn22l IMG_3702.JPG
https://drive.google.com/uc?export=view&id=1PKJyDHTCEGj0VBRpHGuF4akCF58d5A6_ IMG_3703.JPG
https://drive.google.com/uc?export=view&id=1P7kxfSdgCIBjrbeLjjSyIUsjXXFRny7P gikogiko02.jpg
https://drive.google.com/uc?export=view&id=1qJCAIY1slscEVyO0js-K_1m9V6U2BWZe gikogiko01.jpg
https://drive.google.com/uc?export=view&id=1mN4S9YIdgCv7xrDjZUfZNCYWZvPk6k0o unnamed.jpg
https://drive.google.com/uc?export=view&id=1-AJ33Vbq51AJvHLd4eoVGYf7PdD31X68 IMG_3707.JPG
https://drive.google.com/uc?export=view&id=19W3Wri2sfiRAemRsqOcFXE72-CJ7A63e IMG_3708.JPG
https://drive.google.com/uc?export=view&id=1yDSKdBht5OyjoiGsjdb_HhvdWVW1XIDq IMG_3712.JPG
https://drive.google.com/uc?export=view&id=1EIkQQsmZtGoCFg-kKbolYCIV2PJXqP7b IMG_3711.JPG
https://drive.google.com/uc?export=view&id=1UIyENcV4ctywLtrUtf-deBSEpoEzRmtb asobi.jpg
https://drive.google.com/uc?export=view&id=10KC0tPSRsgoDAMTn1vP-WrOQrdHmPeui IMG_3716.JPG
https://drive.google.com/uc?export=view&id=1Ewu6PLKWO77-PzZzIhG-gou-xvyKr4tT IMG_3718.JPG
https://drive.google.com/uc?export=view&id=1UUVAjoYsW348jvy5YNhACUE08qsuieAV IMG_3720.JPG
https://drive.google.com/uc?export=view&id=1cc9prUEmrgvw7dSkI0OfN6JcpeSAfPUW serohan.JPG
https://drive.google.com/uc?export=view&id=14qg8Wtg9Zc8OJ7AtAYM3DAyJsznzGN_R cw.jpg
https://drive.google.com/uc?export=view&id=1fxEruFGVulIwRY2xQwjbk00RItD0bNCU ccw.jpg
https://drive.google.com/uc?export=view&id=1qWPsuiLVGXnUcw1lQfgnoAcF_wK36tRu asobi2.jpg
https://drive.google.com/uc?export=view&id=1k8pxcMDcT4UT6MVvpN3XkhouvbQEjoIT IMG_3726.JPG
https://drive.google.com/uc?export=view&id=1gGwioUOrYLnrHhgIDZgzyZCxp2RGbOl2 all.png
https://drive.google.com/uc?export=view&id=1GXc7Z-VmRg5DAIz07nGuOUdrzeqFysZA IMG_0048.JPG
https://drive.google.com/uc?export=view&id=1SeET5WbTV-mojgHpr7eyNtIcnQEOLmJL IMG_0783.JPG
https://drive.google.com/uc?export=view&id=1g0aUpJN_FavBg12DZqMJQqDU3u4dz8NT IMG_0792.jpg
https://drive.google.com/uc?export=view&id=1qL_pSGlj9fTZf1VZoKK8d8WW6qs6rTY9 IMG_0784.jpg
'

mkdir -p docs/gdrive

echo "$image_table" | while read img_line; do
	[ ! -n "$img_line" ] && continue
	args=($img_line)
	url="${args[0]}"
	out_path="docs/gdrive/${args[1]}"
	echo "$url -> $out_path" >&2
	curl -fsSL -o "$out_path" "$url"
done

cat | jq -c '.[1]'
