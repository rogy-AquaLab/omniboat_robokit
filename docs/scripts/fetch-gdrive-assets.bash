#!/usr/bin/env bash

set -eu

[ $# -gt 1 ] && [ "$1" = "supports" ] && {
    echo $@ >&2
    exit 0
}

image_table='
https://drive.google.com/uc?export=view&id=1FC_E3_lNt-Vslql-hdyLgg66JuzH03v4 IMG_3667.JPG
https://drive.google.com/uc?export=view&id=1U2vqTDOPuZD9QEEEdGIkl6tpNqhT2pd9 IMG_3635.JPG
https://drive.google.com/uc?export=view&id=1hi_kIYDqL_yKklyqnXTGz2I7zg23TVZf IMG_3668.JPG
https://drive.google.com/uc?export=view&id=1-RYB9DSKsES6NLpehvizZpHiYkN1JHVa IMG_0021.JPG
https://drive.google.com/uc?export=view&id=1GGW_c1YBuoTFradbPAQl6WAO_TNDE-Op IMG_0022.JPG
https://drive.google.com/uc?export=view&id=1AXYf3GSSGEAc-TvtGF66c-E9PLfZoruk IMG_0023.JPG
https://drive.google.com/uc?export=view&id=1hzvkm-Bf1BnSCP4HEWZsjR74VfXJ04Bj IMG_3689.JPG
https://drive.google.com/uc?export=view&id=1Npb35MG-aq2LxMMX8n19d1yX1CeyEgN7 IMG_3690.JPG
https://drive.google.com/uc?export=view&id=1Cq1WR2lLlYFa-kK9EaMcAxLu2ZoiOQcq IMG_3691.JPG
https://drive.google.com/uc?export=view&id=1K_l7GEhcdz5SDsnCjA3W6zCYtIeDad5M IMG_3692.JPG
https://drive.google.com/uc?export=view&id=12TCyq-p0uqy2qpbt4mT5crrPXCzqfyFO IMG_3693.JPG
https://drive.google.com/uc?export=view&id=1mpnvrnnIcPQtVj17VWZltOi1QQ9gaqIu IMG_3665.JPG
https://drive.google.com/uc?export=view&id=1j28n2Fjf25fg3IQGWyVHLdNpiWDR_z6L IMG_3694.JPG
https://drive.google.com/uc?export=view&id=1uMejbL8BCIxVa5mvwH-lTg3KuCO9bOYz IMG_3697.JPG
https://drive.google.com/uc?export=view&id=1Iy0lQXBVOD7Gu8ch0KvrnyRegDLWs9vi IMG_3698.JPG
https://drive.google.com/uc?export=view&id=1O3GQymw34T6S-_5q-eeBFtGT9OFTM7VU IMG_3699.JPG
https://drive.google.com/uc?export=view&id=1PXRO4m3ppAGw_kmB3zMkKfOnsR0MK6Jo konomuki.jpg
https://drive.google.com/uc?export=view&id=1uE6p60ABPXqCIqNDfAfguJwFU4ZgMlNe IMG_3701.JPG
https://drive.google.com/uc?export=view&id=10ijLJu2ixbDM4tRipZVegusUL6Bl3YjX IMG_3702.JPG
https://drive.google.com/uc?export=view&id=1koVCEzILTuGv5YgEvTgLBeqpFQD9pkhR IMG_3703.JPG
https://drive.google.com/uc?export=view&id=1CRjTuVdqC-6Rx_-gExBuQrHYKAzsGaxT gikogiko02.jpg
https://drive.google.com/uc?export=view&id=1JWsaeConpXbgAWFu9YxSziA8cFZkM-Lf gikogiko01.jpg
https://drive.google.com/uc?export=view&id=1Y_Q5cUGazeExZGB9psXxlZEUo7hh2n2l unnamed.jpg
https://drive.google.com/uc?export=view&id=19FYj2-HGp9xJAsZqFh_TbrjuWeb5vS_J IMG_3707.JPG
https://drive.google.com/uc?export=view&id=1WwlLTxZ0OwTwfAnIB5ukPytDjrnhd_5u IMG_3708.JPG
https://drive.google.com/uc?export=view&id=1tc2MarCa5yWzAWd07_kJAlh-heKcFDXw omazinai.jpg
https://drive.google.com/uc?export=view&id=1JyaSGN8lEhOiYgWqEzRveX5c_1MTBPzw IMG_3712.JPG
https://drive.google.com/uc?export=view&id=1S99pYE17cfipTyuNkR25-j1vXuFmFjDj IMG_3711.JPG
https://drive.google.com/uc?export=view&id=1r7tXxuuI0_DtB06oXDYYtunY0Fk-Ghmh asobi.jpg
https://drive.google.com/uc?export=view&id=1RneaW1gEe45ueaZKfmTVII7zeRfIshVu IMG_3716.JPG
https://drive.google.com/uc?export=view&id=1IMayZF5Cr7JwSIarzJJFKPFYaJoDcm7o IMG_3718.JPG
https://drive.google.com/uc?export=view&id=1K9oi4Sw7GkLOClQ_KJi70UhLSTC4dKP9 IMG_3720.JPG
https://drive.google.com/uc?export=view&id=1l2x9uLyWvqHJk-Xj8fXW81pFyBDl-a96 serohan.JPG
https://drive.google.com/uc?export=view&id=18Mfm2e0m2bh2TekiILTexJtV4nVHwAv6 cw.jpg
https://drive.google.com/uc?export=view&id=1Di-6vv81J7_Ca4Ef6dOwiuhWa0j273-i ccw.jpg
https://drive.google.com/uc?export=view&id=17aMklspNv6zVNmmgKtdBr_QxhZ5NpJ-Z asobi2.jpg
https://drive.google.com/uc?export=view&id=1a2404i2FkHScXKmjoPOJAZ8ePBcRCWiK IMG_3726.JPG
https://drive.google.com/uc?export=view&id=1WICNJas5Wc4qvqPIjqYbUq-PAnBdyPta all.png
https://drive.google.com/uc?export=view&id=1Bdrs3AB8P31UgQMXyd60Q2NeFKhaejlk IMG_0048.JPG
https://drive.google.com/uc?export=view&id=1g2VHmkvVenEBEJWfBbjGwtJCMLOtGxgp IMG_0783.JPG
https://drive.google.com/uc?export=view&id=1YyF8-rbnftQOYW3ZxE3DI0GHl1BSYQyT IMG_0792.jpg
https://drive.google.com/uc?export=view&id=1Pkl7_I0LfjXmAdgMyfRhl0gCG0nUYlfz IMG_0784.jpg
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
