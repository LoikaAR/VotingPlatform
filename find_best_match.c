









/*
def find_best_match(Ga, Gs, L, xs, ys):
    Ns = build_neighborhood(Gs, L, xs, ys)
    best_pixel = None
    C = np.inf
    for x in range(Ga[L].shape[0]):
        for y in range(Ga[L].shape[1]):
            Na = build_neighborhood(Ga, L, x, y)
            C_new = match_neighborhood(Na, Ns)
            if C_new < C:
                C = C_new
                best_pixel = (x, y)
    return best_pixel
*/