#ifndef IMS_COMPOSE_F_GX_T_H
#define IMS_COMPOSE_F_GX_T_H

namespace ims {

/**
 * @brief Function object unary adapter.
 *
 * Composes two unary operations, using the result of one as input to another.
 * Useful for combining multiple operations in a single function call.
 *
 * @tparam OP1 Type of the outer unary operation
 * @tparam OP2 Type of the inner unary operation
 *
 * @note Inspired by concepts from "The C++ Standard Library" by Nicolai Josuttis,
 *       but updated for modern C++ usage.
 */

template <class OP1, class OP2>
class compose_f_gx_t
{
public:
        // Define parameters
        using argument_type = typename OP2::argument_type;
        using result_type = typename OP1::result_type;
        
private:
        OP1 op1;    // process: op1(op2(x))
        OP2 op2;
public:
        // constructor
        compose_f_gx_t(const OP1& o1, const OP2& o2)
                : op1(o1), op2(o2) {
        }
        
        // function call
        result_type
        operator()(const argument_type& x) const {
                return op1(op2(x));
        }
};

// convenience function for the compose_f_gx adapter
template <class OP1, class OP2>
inline compose_f_gx_t<OP1,OP2>
compose_f_gx (const OP1& o1, const OP2& o2) {
        return compose_f_gx_t<OP1,OP2>(o1,o2);
}

} // namespace ims

#endif // IMS_COMPOSE_F_GX_T_H
