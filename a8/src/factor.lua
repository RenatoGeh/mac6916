local Factor = {}

-- Generates a table in upper with a combination of the possible values.
local function combine(S, i, Pr, upper)
  if #S == 1 then
    local vals = S[1].vals
    for _,v in ipairs(vals) do
      upper[v] = Pr[i]
      i = i + 1
    end
  else
    local vals = S[1].vals
    for _,v in ipairs(vals) do
      upper[v] = {}
      local new_scope = {}
      for _,u in ipairs(S) do
        if u ~= v then
          table.insert(new_scope, u)
        end
      end
      i = combine(new_scope, i, Pr, upper[v])
    end
  end

  return i
end

--[[ Creates a potential given a set of probabilities Pr and the scope S.
--
-- A factor is a table where each probability has a certain instantiation on the variables of S.
--
-- +---------------------+-------+
-- | Valuation V         | Pr(X) |
-- +---------------------+-------+
-- |(X_1=a_1,...,X_n=a_n)|Pr(V_1)|
-- |         ...         |  ...  |
-- |(X_1=z_1,...,X_n=z_n)|Pr(V_n)|
-- +---------------------+-------+
--
-- We structure a potential as a table. Let S be the scope of the potential phi and V_ij be the
-- j-th possible value of variable i. Then we structure phi as:
--
-- phi = {
--   map = {
--     [V_11] = {
--       [V_21] = {
--         ...
--       }
--     },
--     ...
--     [V_1m] = {
--       [V_n1] = {
--         ...
--       }
--     }
--   }
-- }
--
-- To find the probability of a complete state:
--
-- pr = phi.map[x_1][x_2]...[x_n]
--
-- Trying to find a distribution comes naturally, since we get a table:
--
-- Pr = phi.map[x_1][x_2]...[x_m], where m < n
--
-- This only works if the instantiation is ordered.
--]]
function Factor.create(S, Pr)
  local phi = {
    scope = S,
    map = {},
    Pr = Pr,
    _v = {}  -- How much each variable changes each step.
  }

  if combine(S, 1, Pr, phi.map) ~= #Pr + 1 then
    -- Something has gone terribly wrong.
    return nil
  end

  local s = #S
  phi._v[s] = 1
  for i=s-1,1,-1 do
    phi._v[i] = phi._v[i+1] * #(S[i+1].vals)
  end

  return phi
end

function Factor.sum(phi, var)
  -- Constructs new scope without var.
  local phi_scope, new_scope, i_var = phi.scope, {}, 0
  for j,v in ipairs(phi_scope) do
    if v ~= var then
      table.insert(new_scope, v)
    else
      i_var = j
    end
  end

  local P = phi.Pr
  local Q, n = {}, #P
  local vals, n_vals = var.vals, #(var.vals)
  local variance = phi._v[i_var]
  local s = n/n_vals -- Guaranteed for this to be an integer. See /src/cpp/factor.{h,cc}.
  local d = 1

  for i,v in ipairs(phi_scope) do
    d = d * #(v.vals)
    if i == i_var then break end
  end

  for i=1,s,1 do
    Q[i] = 0
  end
  local m = #Q

  local clusters = n/n_vals -- Also guaranteed to be an integer.

  local k=0
  for i=1,clusters,1 do
    local y = (i-1)*n_vals
    for j=1,n_vals,1 do
      Q[k] = Q[k] + P[y + variance]
    end
    k = k + 1
  end

  return Factor.create(new_scope, Q)
end

function Factor.multiply(P)

end

return Factor
