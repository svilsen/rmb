from importlib.metadata import version

from . import _core

def restricted_manhattan_ball(d: int, r: int, km: list[int], kp: list[int], c: list[int], method: str = "exact") -> dict[str, int | float] :
    """Count lattice points of a Manhattan ball in a bounded rectangular grid.

    Counts the number of points in the grid G = {-km_1, ..., kp_1} x ... x {-km_d, ..., kp_d} satisfying sum_i |u_i - c_i| <= r, i.e. the size |B(c | r, G)|.

    Args:
        d: Dimension of the grid.
        r: Radius of the Manhattan ball.
        km: Lower extents k^- = (km_1, ..., km_d).
        kp: Upper extents k^+ = (kp_1, ..., kp_d).
        c: A point of the grid.
        method: Counting method to use:
            - "bruteforce" (or "bf"): enumerate every grid point, exponential in the grid size.
            - "exact" (or "ext"): exact inclusion-exclusion formula, O(d * 3^d), independent of r.
            - "approximate" (or "app"): leading-term estimate of the exact formula, O(d * 3^d), independent of r, with a guaranteed absolute error bound.

    Returns:
        A dict with keys:
            - "count": the number of points (exact for "bruteforce"/"exact"; estimate for "approximate").
            - "error": 0 for "bruteforce"/"exact"; the guaranteed bound on |count - true value| for "approximate".
    """
    ##
    #
    if (len(km) != d) or (len(kp) != d) or (len(c) != d) :
        raise ValueError(
            f"'km', 'kp' and 'c' must each have length d = {d}; got len(km) = {len(km)}, len(kp) = {len(kp)}, len(c) = {len(c)}."
        )

    ##
    #
    if method == "bf" or method == "bruteforce" :
        count = _core.bruteforce(d, r, km, kp, c)
        error = 0
    elif method == "ext" or method == "exact":
        count = _core.exact(d, r, km, kp, c)
        error = 0
    elif method == "app" or method == "approximate":
        count, error = _core.approximate(d, r, km, kp, c)
    else :
        raise ValueError(
            f"'method' only takes the values: 'bruteforce' (or 'bf'), 'exact' (or 'ext'), and 'approximate' (or 'app'); got the value '{method}'"
        )

    ##
    #
    return {
        "count": count,
        "error": error
    }

__version__ = version("rmb")
__all__ = ["restricted_manhattan_ball"]
