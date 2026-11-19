from importlib.metadata import version

from . import _core

def restricted_manhattan_ball(d: int, r: int, km: list[int], kp: list[int], c: list[int], method: str = "dynamic", type: bool = True) -> int | tuple[int, float] :
    """Count lattice points of a Manhattan ball in a bounded rectangular grid.

    Counts the number of points in the grid G = {-km_1, ..., kp_1} x ... x {-km_d, ..., kp_d} satisfying sum_i |u_i - c_i| <= r, i.e. the size |B(c | r, G)|.

    Args:
        d: Dimension of the grid.
        r: Radius of the Manhattan ball.
        km: Lower extents k^- = (km_1, ..., km_d).
        kp: Upper extents k^+ = (kp_1, ..., kp_d).
        c: A point of the grid.
        method: The counting method,
            - "bruteforce" (or "bf"): enumerate every grid point, exponential in the grid size.
            - "exact" (or "ext"): exact inclusion-exclusion formula, O(d * 3^d) time-complexity.
            - "dynamic" (or "dp"): exact inclusion-exclusion formula, O(d * r) time-complexity and O(r) memory-complexity, found using dynamic programming.
            - "approximate" (or "app"): leading-term estimate of the exact formula, O(d * 3^d) time, with a guaranteed absolute error bound.
        type: Should the terms be grouped? NB: when 'method' == "bruteforce", this term is ignored.

    Returns:
        If 'method' is "bruteforce", "exact", or "dynamic", an 'int' is returned:
            - "count": an exact count of the number of points.
        If 'method' is "approximate" a 'tuple[int, float]' is returned:
            - "count": an estimate of the number of points.
            - "error": the guaranteed bound on |count - true value| for "approximate".
    """
    ##
    #
    if (len(km) != d) or (len(kp) != d) or (len(c) != d) :
        raise ValueError(
            f"'km', 'kp' and 'c' must each have length d = {d}; got len(km) = {len(km)}, len(kp) = {len(kp)}, len(c) = {len(c)}."
        )

    ##
    #
    error = 0
    if method == "bf" or method == "bruteforce" :
        count = _core.bruteforce(d, r, km, kp, c)
    elif method == "ext" or method == "exact":
        if type :
            count = _core.grouped_exact(d, r, km, kp, c)
        else :
            count = _core.exact(d, r, km, kp, c)
    elif method == "dp" or method == "dynamic":
        if type :
            count = _core.grouped_dp(d, r, km, kp, c)
        else :
            count = _core.dp(d, r, km, kp, c)
    elif method == "app" or method == "approximate":
        if type :
            count, error = _core.grouped_approximate(d, r, km, kp, c)
        else :
            count, error = _core.approximate(d, r, km, kp, c)
    else :
        raise ValueError(
            f"'method' only takes the values: 'bruteforce' (or 'bf'), 'exact' (or 'ext'), and 'approximate' (or 'app'); got the value '{method}'"
        )

    ##
    #
    if error > 1e-16 :
        return count, error
    else :
        return count

__version__ = version("rmb")
__all__ = ["restricted_manhattan_ball"]
