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
    _v = {}  -- How much each variable changes each step (stride).
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
  -- Create new scope and probability distribution.
  local new_S, new_Pr = {}, {}
  for i,v in ipairs(phi.scope) do
    if v ~= var then table.insert(new_S, v)
    else 
  end
end

function Factor.multiply(P)

end

return Factor
