function gray(brightness)
	b = 255 * brightness
	return {"rgba",b,b,b,255}
end

function grayAlpha(brightness, solidity)
	b = 255 * brightness
	return {"rgba",b,b,b,solidity}
end

grey = gray; greyAlpha = grayAlpha

function rgb(r,g,b)
	return {"rgba",r,g,b,255}
end

function rgba(r,g,b,a)
	return {"rgba",r,g,b,a}
end

function hsv(h,s,v)
	return {"hsva",h,s,b,255}
end

function hsva(h,s,v,a)
	return {"hsva",h,s,v,a}
end

function merge_settings() 
	for k,v in pairs(user_settings) do
		default_settings[k] = v
	end
end




function register_strings()
	for k,v in pairs(local_language_strings) do
		register_string(k,v)
	end
end

function register_settings()
	for k,v in pairs(default_settings) do
		if (type(v) == "table") then
			if     (v[1] == "rgba") then register_RGBA_color(k,v[2],v[3],v[4],v[5])
			elseif (v[1] == "hsva") then register_HSVA_color(k,v[2],v[3],v[4],v[5])
			end
		elseif (type(v) == "number")  then register_number(k,v)
		elseif (type(v) == "string")  then register_string(k,v)
		elseif (type(v) == "boolean") then register_boolean(k,v)
		end
	end
end

