-- BigInt precedures
-- a BigInt that will work on any value
CREATE OR REPLACE FUNCTION bigint_working(val BIGINT)
   RETURNS BIGINT AS $$
    return val - 1n;
   $$ LANGUAGE plv8 STABLE STRICT;
SELECT bigint_working(9223372036854775807);

SELECT bigint_working(32);

-- a BigInt that will fail on any value
CREATE OR REPLACE FUNCTION bigint_failing(val BIGINT)
   RETURNS BIGINT AS $$
    return val - 1;
   $$ LANGUAGE plv8 STABLE STRICT;
SELECT bigint_failing(9223372036854775807);
