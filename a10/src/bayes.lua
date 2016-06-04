local Bayes = {}

InteractionGraph = require "interaction_graph"
Sampling = require "sampling"

function Bayes.create(Sc, P)
  local N = {
    Sc = nil,
    P = nil,
    G = nil
  }

  N.Sc, N.P = Sc, P

  -- TODO: Create graph G.

  return N
end

function Bayes:evidence(N, E, func, m)
  func = func or Sampling.logical
  m = m or 10000

  return func(N.Sc, N.P, E, m)
end

return Bayes
