-- Module init.
local Utils = {}

local MAX_INT = 2^51+1

-- Pseudo-random number (0, 1).
-- Considers 64-bits integer.
function Utils.random()
  return math.random(0, MAX_INT)/MAX_INT
end

-- Table contains val?
function Utils.contains_all(table, val)
  for _,v in pairs(table) do
    if v == val then
      return true
    end
  end
  return false
end

-- Array table contains val?
function Utils.contains(table, val)
  for _,v in ipairs(table) do
    if v == val then
      return true
    end
  end
  return false
end

return Utils
