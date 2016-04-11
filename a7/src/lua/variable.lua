local Variable = {}

-- Number of variables created. A variable should only be created once.
local __g_var_c = 0

function Variable.create(n_vals)
  local v = {
    id = __g_var_c,  -- ID
    vals = {},       -- Possible values
    n = n_vals       -- Number of possible values
  }

  __g_var_c = __g_var_c + 1

  for i=1,n_vals,1 do
    v[i] = i-1
  end

  return v
end
