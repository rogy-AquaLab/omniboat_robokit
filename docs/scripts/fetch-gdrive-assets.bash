#!/usr/bin/env bash

set -eu

[ $# -gt 1 ] && [ "$1" = "supports" ] && {
    echo $@ >&2
    exit 0
}

image_table='
https://drive.google.com/uc?export=view&id=1FC_E3_lNt-Vslql-hdyLgg66JuzH03v4 machine_motorcheck.JPG
https://drive.google.com/uc?export=view&id=1U2vqTDOPuZD9QEEEdGIkl6tpNqhT2pd9 machine_servohorn_1.JPG
https://drive.google.com/uc?export=view&id=1hi_kIYDqL_yKklyqnXTGz2I7zg23TVZf machine_servohorn_2.JPG
https://drive.google.com/uc?export=view&id=1-RYB9DSKsES6NLpehvizZpHiYkN1JHVa machine_board2.JPG
https://drive.google.com/uc?export=view&id=1GGW_c1YBuoTFradbPAQl6WAO_TNDE-Op machine_motorcover.JPG
https://drive.google.com/uc?export=view&id=1AXYf3GSSGEAc-TvtGF66c-E9PLfZoruk machine_pillar.JPG
https://drive.google.com/uc?export=view&id=1hzvkm-Bf1BnSCP4HEWZsjR74VfXJ04Bj machine_board1_1.JPG
https://drive.google.com/uc?export=view&id=1Npb35MG-aq2LxMMX8n19d1yX1CeyEgN7 machine_board1_2.JPG
https://drive.google.com/uc?export=view&id=1Cq1WR2lLlYFa-kK9EaMcAxLu2ZoiOQcq machine_board1_3.JPG
https://drive.google.com/uc?export=view&id=1K_l7GEhcdz5SDsnCjA3W6zCYtIeDad5M machine_board1_4.JPG
https://drive.google.com/uc?export=view&id=12TCyq-p0uqy2qpbt4mT5crrPXCzqfyFO machine_sillicontube.JPG
https://drive.google.com/uc?export=view&id=1mpnvrnnIcPQtVj17VWZltOi1QQ9gaqIu machine_pulley1_pressfit_1.JPG
https://drive.google.com/uc?export=view&id=1j28n2Fjf25fg3IQGWyVHLdNpiWDR_z6L machine_pulley1_pressfit_2.JPG
https://drive.google.com/uc?export=view&id=1uMejbL8BCIxVa5mvwH-lTg3KuCO9bOYz machine_motor_shaft_stopper.JPG
https://drive.google.com/uc?export=view&id=1Iy0lQXBVOD7Gu8ch0KvrnyRegDLWs9vi machine_motor_coupling_1.JPG
https://drive.google.com/uc?export=view&id=1O3GQymw34T6S-_5q-eeBFtGT9OFTM7VU machine_motor_coupling_2.JPG
https://drive.google.com/uc?export=view&id=1PXRO4m3ppAGw_kmB3zMkKfOnsR0MK6Jo machine_konomuki.jpg
https://drive.google.com/uc?export=view&id=1uE6p60ABPXqCIqNDfAfguJwFU4ZgMlNe machine_crankshaft.JPG
https://drive.google.com/uc?export=view&id=10ijLJu2ixbDM4tRipZVegusUL6Bl3YjX machine_pulley2.JPG
https://drive.google.com/uc?export=view&id=1koVCEzILTuGv5YgEvTgLBeqpFQD9pkhR machine_propeller_burs.JPG
https://drive.google.com/uc?export=view&id=1CRjTuVdqC-6Rx_-gExBuQrHYKAzsGaxT machine_gikogiko_1.jpg
https://drive.google.com/uc?export=view&id=1JWsaeConpXbgAWFu9YxSziA8cFZkM-Lf machine_gikogiko_2.jpg
https://drive.google.com/uc?export=view&id=1Y_Q5cUGazeExZGB9psXxlZEUo7hh2n2l machine_propeller_fins.jpg
https://drive.google.com/uc?export=view&id=19FYj2-HGp9xJAsZqFh_TbrjuWeb5vS_J machine_construct_propeller_1.JPG
https://drive.google.com/uc?export=view&id=1WwlLTxZ0OwTwfAnIB5ukPytDjrnhd_5u machine_construct_propeller_2.JPG
https://drive.google.com/uc?export=view&id=1tc2MarCa5yWzAWd07_kJAlh-heKcFDXw machine_omajinai.jpg
https://drive.google.com/uc?export=view&id=1JyaSGN8lEhOiYgWqEzRveX5c_1MTBPzw machine_construct_propeller_3.JPG
https://drive.google.com/uc?export=view&id=1S99pYE17cfipTyuNkR25-j1vXuFmFjDj machine_construct_propeller_4.JPG
https://drive.google.com/uc?export=view&id=1r7tXxuuI0_DtB06oXDYYtunY0Fk-Ghmh machine_asobi_1.jpg
https://drive.google.com/uc?export=view&id=1RneaW1gEe45ueaZKfmTVII7zeRfIshVu machine_propeller_1.JPG
https://drive.google.com/uc?export=view&id=1IMayZF5Cr7JwSIarzJJFKPFYaJoDcm7o machine_propeller_2.JPG
https://drive.google.com/uc?export=view&id=1K9oi4Sw7GkLOClQ_KJi70UhLSTC4dKP9 machine_propeller_3.JPG
https://drive.google.com/uc?export=view&id=1l2x9uLyWvqHJk-Xj8fXW81pFyBDl-a96 machine_serohan.jpg
https://drive.google.com/uc?export=view&id=18Mfm2e0m2bh2TekiILTexJtV4nVHwAv6 machine_propeller_cw.jpg
https://drive.google.com/uc?export=view&id=1Di-6vv81J7_Ca4Ef6dOwiuhWa0j273-i cmachine_propeller_cw.jpg
https://drive.google.com/uc?export=view&id=17aMklspNv6zVNmmgKtdBr_QxhZ5NpJ-Z machine_asobi_2.jpg
https://drive.google.com/uc?export=view&id=1a2404i2FkHScXKmjoPOJAZ8ePBcRCWiK machine_thruster_complete.JPG
https://drive.google.com/uc?export=view&id=1WICNJas5Wc4qvqPIjqYbUq-PAnBdyPta machine_structure_abstract.png
https://drive.google.com/uc?export=view&id=1Bdrs3AB8P31UgQMXyd60Q2NeFKhaejlk machine_structure_real.jpg
https://drive.google.com/uc?export=view&id=1g2VHmkvVenEBEJWfBbjGwtJCMLOtGxgp machine_waterproof_1.JPG
https://drive.google.com/uc?export=view&id=1YyF8-rbnftQOYW3ZxE3DI0GHl1BSYQyT machine_waterproof_2.jpg
https://drive.google.com/uc?export=view&id=1Pkl7_I0LfjXmAdgMyfRhl0gCG0nUYlfz machine_complete.jpg
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
