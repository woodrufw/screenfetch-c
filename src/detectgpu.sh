#!/usr/bin/env bash
#  detectgpu
#  attempt to detect and output the name and model of the computer's GPU
#  only meant to be run on Linux/*BSD
#  Original Author: Brett Bohnenkamper (KittyKatt) in screenfetch-dev
#  Modified to stand alone by William Woodruff (woodruffw) in screenfetch-c

if [ -n "$(type -p lspci)" ]; then
	gpu_info=$(lspci 2> /dev/null | grep VGA)
	gpu=$(echo "$gpu_info" | grep -oE '\[.*\]' | sed 's/\[//;s/\]//')
	gpu=$(echo "${gpu}" | sed -n '1h;2,$H;${g;s/\n/, /g;p}')
elif [[ -n "$(type -p glxinfo)" && -z "$gpu" ]]; then
	gpu_info=$(glxinfo 2>/dev/null)
	gpu=$(echo "$gpu_info" | grep "OpenGL renderer string")
	gpu=$(echo "$gpu" | cut -d ':' -f2)
	gpu="${gpu:1}"
	gpu_info=$(echo "$gpu_info" | grep "OpenGL vendor string")
fi

if [ -n "$gpu" ];then
	if [ $(echo "$gpu_info" | grep -i nvidia | wc -l) -gt 0 ] ; then
		gpu_info="NVidia "
	elif [ $(echo "$gpu_info" | grep -i intel | wc -l) -gt 0 ] ; then
		gpu_info="Intel "
	elif [ $(echo "$gpu_info" | grep -i amd | wc -l) -gt 0 ] ; then
		gpu_info="AMD "
	elif [[ $(echo "$gpu_info" | grep -i ati | wc -l) -gt 0  || $(echo "$gpu_info" | grep -i radeon | wc -l) -gt 0 ]] ; then
		gpu_info="ATI "
	else
		gpu_info=$(echo "$gpu_info" | cut -d ':' -f2)
		gpu_info="${gpu_info:1} "
	fi
	gpu="${gpu}"
else
	gpu="Unknown"
fi

echo $gpu